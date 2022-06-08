#Secondary library for valid chord progressions. Used with courtesy of Ryan He from S22-allolib.

all_chords = ["I", "ii", "iii", "IV", "V", "vi", "viid", "I6", "ii6", "iii6", "IV6", "V6", "vi6", "viid6", "I64", "ii64", "iii64", "IV64", "V64", "vi64", "viid64"]
major = {
      "I" : ["I", "ii", "iii", "IV", "V", "vi", "viid", "I6", "ii6", "iii6", "IV6", "V6", "vi6", "viid6", "I64", "ii64", "iii64", "IV64", "V64", "vi64", "viid64"],
      "I6" : ["I", "ii", "iii", "IV", "V", "vi", "viid", "I6", "ii6", "iii6", "IV6", "V6", "vi6", "viid6", "I64", "ii64", "iii64", "IV64", "V64", "vi64", "viid64"],
      "I64" : ["I", "ii", "iii", "IV", "V", "vi", "viid", "I6", "ii6", "iii6", "IV6", "V6", "vi6", "viid6", "I64", "ii64", "iii64", "IV64", "V64", "vi64", "viid64"],
      "ii" : ["I", "V", "viid", "I6", "V6", "viid6", "I64", "V64", "viid64"],
      "ii6" : ["I", "V", "viid", "I6", "V6", "viid6", "I64", "V64", "viid64"],
      "ii64" : ["I", "V", "viid", "I6", "V6", "viid6", "I64", "V64", "viid64"],
      "iii" : ["I", "ii", "IV", "vi", "I6", "ii6", "IV6", "vi6", "I64", "ii64", "IV64", "vi64"],
      "iii6" : ["I", "ii", "IV", "vi", "I6", "ii6", "IV6", "vi6", "I64", "ii64", "IV64", "vi64"],
      "iii64" : ["I", "ii", "IV", "vi", "I6", "ii6", "IV6", "vi6", "I64", "ii64", "IV64", "vi64"],
      "IV" : ["I", "ii", "iii", "V", "viid", "I6", "ii6", "iii6", "V6", "viid6", "I64", "ii64", "iii64", "V64", "viid64"],
      "IV6" : ["I", "ii", "iii", "V", "viid", "I6", "ii6", "iii6", "V6", "viid6", "I64", "ii64", "iii64", "V64", "viid64"],
      "IV64" : ["I", "ii", "iii", "V", "viid", "I6", "ii6", "iii6", "V6", "viid6", "I64", "ii64", "iii64", "V64", "viid64"],
      "V" : ["I", "vi", "I6", "vi6", "I64", "vi64"],
      "V6" : ["I", "vi", "I6", "vi6", "I64", "vi64"],
      "V64" : ["I", "vi", "I6", "vi6", "I64", "vi64"],
      "vi" : ["I", "ii", "iii", "IV", "V", "I6", "ii6", "iii6", "IV6", "V6", "I64", "ii64", "iii64", "IV64", "V64"],
      "vi6" : ["I", "ii", "iii", "IV", "V", "I6", "ii6", "iii6", "IV6", "V6", "I64", "ii64", "iii64", "IV64", "V64"],
      "vi64" : ["I", "ii", "iii", "IV", "V", "I6", "ii6", "iii6", "IV6", "V6", "I64", "ii64", "iii64", "IV64", "V64"],
      "viid" : ["I", "iii", "I6", "iii6", "I64", "iii64"],
      "viid6" : ["I", "iii", "I6", "iii6", "I64", "iii64"],
      "viid64" : ["I", "iii", "I6", "iii6", "I64", "iii64"]   
}

'''
def chordToNotes(chord): #Deprecated
    if (chord == "I"):
        return ["C", "E", "G"]
    if (chord == "I6"):
        return ["E", "G", "C"]
    if (chord == "I64"):
        return ["G", "C", "E"]

    if (chord == "ii"):
        return ["D", "F", "A"]
    if (chord == "ii6"):
        return ["F", "A", "D"]
    if (chord == "ii64"):
        return ["A", "D", "F"]

    if (chord == "iii"):
        return ["E", "G", "B"]
    if (chord == "iii6"):
        return ["G", "B", "E"]
    if (chord == "iii64"):
        return ["B", "E", "G"]

    if (chord == "IV"):
        return ["F", "A", "C"]
    if (chord == "IV6"):
        return ["A", "C", "F"]
    if (chord == "IV64"):
        return ["C", "F", "A"]

    if (chord == "V"):
        return ["G", "B", "D"]
    if (chord == "V6"):
        return ["B", "D", "G"]
    if (chord == "V64"):
        return ["D", "G", "B"]

    if (chord == "vi"):
        return ["A", "C", "E"]
    if (chord == "vi6"):
        return ["C", "E", "A"]
    if (chord == "vi64"):
        return ["E", "A", "C"]

    if (chord == "viid"):
        return ["B", "D", "F"]
    if (chord == "viid6"):
        return ["D", "F", "B"]
    if (chord == "viid64"):
        return ["F", "B", "D"]
'''

chord_notes = {
    "I" : ["C", "E", "G"],
    "I6" : ["E", "G", "C"],
    "I64": ["G", "C", "E"],
    "ii": ["D", "F", "A"],
    "ii6": ["F", "A", "D"],
    "ii64": ["A", "D", "F"],
    "iii": ["E", "G", "B"],
    "iii6": ["G", "B", "E"],
    "iii64": ["B", "E", "G"],
    "IV": ["F", "A", "C"],
    "IV6": ["A", "C", "F"],
    "IV64": ["C", "F", "A"],
    "V": ["G", "B", "D"],
    "V6": ["B", "D", "G"],
    "V64": ["D", "G", "B"],
    "vi": ["A", "C", "E"],
    "vi6": ["C", "E", "A"],
    "vi64": ["E", "A", "C"],
    "viid": ["B", "D", "F"],
    "viid6": ["D", "F", "B"],
    "viid64": ["F", "B", "D"]
}
def chordToNotes(chord):
    if chord in all_chords:
        return chord_notes[chord]
    else:
        return []

note_map = {
    "C" : 0,
    "D" : 2,
    "E" : 4,
    "F" : 5,
    "G" : 7,
    "A" : 9,
    "B" : 11
}

ascii_map = {
    "0" : "C",
    "2" : "D",
    "4" : "E",
    "5" : "F",
    "7" : "G",
    "9" : "A",
    "11" : "B"
}

#Returns a dictionary containing all valid chords to jump to, keyed by the middle note of the chord.
def get_next_set(chord_previous):
    if chord_previous in all_chords:
        new_set = major[chord_previous]

        selection = {}
        for i in new_set:
            notes = chordToNotes(i)
            if str(notes[1]) in selection.keys():
                selection[str(notes[1])].append(i)
            else:
                selection[str(notes[1])] = [i]
        
        return selection
    
        
        
