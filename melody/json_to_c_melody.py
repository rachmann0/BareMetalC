import json
import os

# -----------------------------
# CONFIG: adjust these
# -----------------------------
TEMPO_BPM = 90  # beats per minute
TONIC = 'C'      # default tonic if not in JSON
F_CPU = 16_000_000  # Arduino clock
PRESCALER = 1      # timer prescaler used in GET_COMPARE_VAL

# JSON input file
INPUT_JSON = "ocarina_of_time.json"
script_dir = os.path.dirname(__file__)  # scripts/
output_dir = os.path.join(script_dir, "..", "include")  # ../include/
os.makedirs(output_dir, exist_ok=True)

OUTPUT_H_FILE = os.path.join(output_dir, "ocarina_of_time.h")


# -----------------------------
# SCALE DEGREE TO SEMITONE
# 1 = tonic, 2 = major second, ... 7 = major seventh
# b2 = flat second
# -----------------------------
DEGREE_TO_SEMITONE = {
    '1': 0,
    'b2': 1,
    '2': 2,
    'b3': 3,
    '3': 4,
    '4': 5,
    'b5': 6,
    '5': 7,
    'b6': 8,
    '6': 9,
    'b7': 10,
    '7': 11
}

# -----------------------------
# NOTE NAME TO MIDI BASE (for tonic C)
# -----------------------------
NOTE_TO_MIDI = {
    'C': 0,
    'C#': 1,
    'D': 2,
    'D#': 3,
    'E': 4,
    'F': 5,
    'F#': 6,
    'G': 7,
    'G#': 8,
    'A': 9,
    'A#': 10,
    'B': 11
}

# -----------------------------
# FUNCTIONS
# -----------------------------
def sd_to_freq(sd, octave, tonic):
    """Convert scale degree + octave + tonic to frequency in Hz"""
    semitone_offset = DEGREE_TO_SEMITONE[sd] + octave * 12
    tonic_midi = NOTE_TO_MIDI[tonic.upper()]
    midi_note = tonic_midi + semitone_offset
    # frequency formula: f = 440 * 2^((n-69)/12)
    freq = 440.0 * 2 ** ((midi_note - 69)/12)
    return freq

def get_compare_val(freq):
    """Calculate OCR1A value"""
    return int(F_CPU / (2 * PRESCALER * freq) - 1)

def beats_to_ms(beats, tempo):
    """Convert beats to milliseconds"""
    beat_length = 60.0 / tempo  # seconds per beat
    return int(beats * beat_length * 1000)

# -----------------------------
# LOAD JSON
# -----------------------------
with open(INPUT_JSON, 'r') as f:
    data = json.load(f)

# get tonic from JSON keys if available
if 'keys' in data and len(data['keys']) > 0:
    TONIC = data['keys'][0].get('tonic', TONIC)

# -----------------------------
# GENERATE C ARRAY
# -----------------------------
lines = []

lines.append('#ifndef OCARINA_OF_TIME_H')
lines.append('#define OCARINA_OF_TIME_H')

lines.append('#include "tone.h"')
lines.append('#include "music.h"')
lines.append('#include <avr/pgmspace.h>')
lines.append('const Note melody[] PROGMEM = {')

# inside the loop over notes
for note in data['notes']:
    if note['isRest']:
        # Only for rests, write plain 0
        lines.append(f'    {{0, {beats_to_ms(note["duration"], TEMPO_BPM)}}},  // {note["sd"]}{note["octave"]} rest')
    else:
        freq = sd_to_freq(note['sd'], note['octave'] + 4, TONIC)
        # ocr = get_compare_val(freq)
        # lines.append(f'    {{{ocr}, {beats_to_ms(note["duration"], TEMPO_BPM)}}},  // {note["sd"]}{note["octave"]}')
        lines.append(f'    {{GET_COMPARE_VAL({freq}), {beats_to_ms(note["duration"], TEMPO_BPM)}}},  // {note["sd"]}{note["octave"]}')

lines.append('};')
lines.append(f'const uint8_t melody_length = sizeof(melody)/sizeof(melody[0]);')

lines.append('#endif')

# -----------------------------
# SAVE TO FILE
# -----------------------------
with open(OUTPUT_H_FILE, 'w') as f:
    f.write('\n'.join(lines))

print(f"C header generated: {OUTPUT_H_FILE}")