from printer_utilities import Slicer, OBJGeometry

geometry = OBJGeometry('item.obj')

slicer = Slicer(geometry, dots_per_side = 16)
layers = slicer.slice(thickness = 1.0)

for lay in layers:
    print(lay)
    print('\n')