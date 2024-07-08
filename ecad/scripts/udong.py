'''
import pcbnew
import os
import sys
sys.path.append(os.path.join(os.path.dirname(pcbnew.GetBoard().GetFileName()), '../scripts/'))
import udong
'''

import pcbnew

import sys
import re

def reload():
    import importlib
    importlib.reload(sys.modules[__name__])

digital_switch_positions = [
    [64, 56],
    [94, 58],
    [122, 76],
    [34, 58],

    [122, 106],
    [34, 90],
    [94, 90],
    [64, 88],

    [-126, 102],
    [20, 128],
    [6, 72],
    [-70, 50],

    [-10, 138],
    [-100, 78],
    [-40, 78],
    [-70, 76],
]

analog_switch_positions = [
    [122, 26],
    [94, 26],
]


'''
returns a dict contains all digital switch related foot prints.

ret[0]['BC'] = <footprint of Bypass Condensor for Switch-0>
ret[0]['OR'] = <a resitor for low-pass filter at output>
ret[0]['OC'] = <a capacitor for low-pass filter at output>
ret[1]...
'''
def list_switches():
    board = pcbnew.GetBoard()
    switches = {}

    splitter = re.compile('([A-Za-z]+)\s*(\d+)')

    for fp in board.GetFootprints():
        if fp.HasField("DigitalSwitch"):
            field = fp.GetFieldByName("DigitalSwitch")
            field_value = field.GetText()
            print(field_value)
            match = splitter.fullmatch(field_value)
            if match == None:
                raise ValueError(f"Invalid 'DigitalSwitch' field value '{field_value}' in {fp.GetReference()}.")
            part_type = match.groups()[0]
            switch_id = int(match.groups()[1])
            print(f"   type: {part_type}, switch_id: {switch_id}")

            if not switch_id in switches:
                switches[switch_id] = {}
            switches[switch_id][part_type] = fp

    return switches

'''
takes a dict of footprints for DigitalSwitch-0
returns a dict of relative positions from the switch footprint(K)
{
  'K': [0, 0],
  'BC': [-1111, 1111],
  'OC': [22, 333],
  'OR': [444, 4444],
}

'''
def calc_rel_pos(d):
    if not 'K' in d:
        raise ValueError('DigitalSwitch-0 does not have a footprint w/ a field DigitalSwitch=K0')
    switch = d['K']

    base_x = switch.GetX()
    base_y = switch.GetY()
    print(f"K0 absolute position:{base_x}, {base_y}")

    print("Listing relative positions")
    ret = {}
    for t in d:
        rel_x = d[t].GetX() - base_x
        rel_y = d[t].GetY() - base_y
        ori = d[t].GetOrientationDegrees()
        print(f"    {t}0 :{rel_x}nm, {rel_y}nm")
        ret[t] = [rel_x, rel_y, ori]

    return ret

def mm_to_nm(mm):
    return mm * 1000 * 1000

def update_digital_switches(switch_pos_in_millis):
    board = pcbnew.GetBoard()

    origin = board.GetDesignSettings().GetGridOrigin()

    switches = list_switches()
    print("Switch 0")
    rel_pos_dict = calc_rel_pos(switches[0])

    for switch_id in range(0, 16):
        # Convert mm to nm
        base_x = mm_to_nm(switch_pos_in_millis[switch_id][0])
        base_y = mm_to_nm(switch_pos_in_millis[switch_id][1])
        sw_parts = switches[switch_id]
        for part_type in sw_parts:
            print(f"  type: {part_type}")

            rel_pos = rel_pos_dict[part_type]
            print(f"  rel-pos: {rel_pos}")
            # Note that x-axis is reversed since we put parts on the back
            # side of PCB
            new_x = origin.x - base_x + rel_pos[0]
            new_y = origin.y + base_y + rel_pos[1]
            new_ori = rel_pos[2]
            print(f"  new_pos: {new_x}, {new_y}, orientatin: {new_ori}")

            part = sw_parts[part_type]
            part.SetX(new_x)
            part.SetY(new_y)
            part.SetOrientationDegrees(new_ori)

def update_switches():
    try:
        update_digital_switches(digital_switch_positions)
    except Exception as e:
        print(e)

def list_holes():
    board = pcbnew.GetBoard()
    holes = []

    hole_name = "Udong:SK20-keyswitch-rounded-hole"
    for fp in board.GetFootprints():
        if fp.GetFieldByName("Footprint").GetText() == hole_name:
            holes.append(fp)

    return holes

def update_holes(switch_pos_in_millis):
    holes = list_holes()
    assert(len(holes) == len(switch_pos_in_millis))

    board = pcbnew.GetBoard()
    origin = board.GetDesignSettings().GetGridOrigin()
    for i in range(len(holes)):
        switch_x = mm_to_nm(switch_pos_in_millis[i][0])
        switch_y = mm_to_nm(switch_pos_in_millis[i][1])

        x = origin.x - switch_x
        y = origin.y + switch_y

        holes[i].SetX(x)
        holes[i].SetY(y)

try:
    update_holes(digital_switch_positions + analog_switch_positions)
except Exception as e:
    print(e)
