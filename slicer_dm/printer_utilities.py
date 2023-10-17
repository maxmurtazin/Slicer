from enum import Enum
from typing import Dict, Tuple, List

Line = Tuple[Tuple[float, float],
             Tuple[float, float]]

Vertice = Tuple[float, float, float]
Triangle = Tuple[int, int, int]

class CellType(Enum):
    empty = 0
    filled = 1
    marked = 2

class Canvas:
    def __init__(self, width: int, height: int) -> None:
        assert width > 0
        assert height > 0
        
        self.width = width
        self.height = height
        self.cells = [[CellType.empty]*width for i in range(height)]
    
    def __getitem__(self, position: Tuple[int, int]) -> CellType:
        x, y = position
        return self.cells[y][x]
    
    def __setitem__(self, position: Tuple[int, int], value: CellType) -> None:
        x, y = position
        self.cells[y][x] = value
    
    def __str__(self):
        return '\n'.join(['%d '*self.width % tuple([cell.value \
                                                    for cell in row]) \
                          for row in self.cells])
    
    def merge(self, other) -> None:
        assert self.width == other.width
        assert self.height == other.height

        for y in range(self.height):
            for x in range(self.width):
                if other[(x, y)] == CellType.filled:
                    self[(x, y)] = CellType.filled
    
    def add_lines(self, lines: List[Line]) -> None:
        self.lines = lines
    
    def cell_on_edge(self, x: int, y: int) -> bool:
        return (x == 0) or \
               (y == 0) or \
               (x == self.width-1) or \
               (y == self.height-1)
    
    def empty_cells_among_neighbors(self, x: int, y: int) -> bool:
        neighbors = [(x, y-1), (x-1, y), (x, y+1), (x+1, y)]
        
        return any(self[n] == CellType.empty for n in neighbors)
    
    def draw_line(self, line: Line) -> None:
        # The function rasterizes the line by selecting points
        # with integer coordinates that best obey the equation of the line.
        
        x0, y0, x1, y1 = map(int, (*line[0], *line[1]))
        x, y = x0, y0
        
        dx = 2*(x1 > x0) - 1
        dy = 2*(y1 > y0) - 1

        while True:
            self[(x, y)] = CellType.filled
            
            if x == x1 and y == y1:
                break
                
            errx = abs((y1 - y0)*(x+dx-x0) + (y0 - y)*(x1 - x0))
            erry = abs((y1 - y0)*(x-x0) + (y0 - y-dy)*(x1 - x0))
            
            x += (errx < erry)*dx
            y += (errx >= erry)*dy

    def fill_holes(self) -> None:
        # The function fills closed contours in the 'cellular automata' style:
        # any cell adjacent to empty ones becomes empty.
        # The cell pointer moves in a spiral from the top left corner
        # to the center of the canvas. Thanks to this, pouring does not
        # require multiple passes across the canvas.
        
        borders = [-1, -1, self.width, self.height] # left, top, right, bottom
        coord = [0, 0] # start from upper left corner
        steps = [1, 1, -1, -1] # l->r, t->b, r->l, b->t
        
        # first we move to the right
        coord_idx = 0 # for this reason, we change the X coordinate first
        border_idx = 2 # accordingly, we check the right border
        step_idx = 0 # left->right step
        
        # first mark all empty cells
        self.cells = [[CellType.marked if cell == CellType.empty \
                                       else cell \
                                       for cell in row] \
                                       for row in self.cells]
        
        while True:
            # sequentially clear all marked cells
            # if they are on the edge of the canvas or adjacent to empty ones
            if self[coord] == CellType.marked and \
               (self.cell_on_edge(coord[0], coord[1]) or \
                self.empty_cells_among_neighbors(coord[0], coord[1])):
                   self[coord] = CellType.empty
            
            coord[coord_idx] += steps[step_idx]
            
            # if we come across a border, we shift it in the direction
            # opposite to the movement (for the next loop of the spiral)
            # and turn clockwise
            if coord[coord_idx] + steps[step_idx] == borders[border_idx]:
                borders[border_idx] += -steps[step_idx]
                
                coord_idx = (coord_idx + 1) % len(coord)
                step_idx = (step_idx + 1) % len(steps)
                border_idx = (border_idx + 1) % len(borders)
                
                # if after turning we hit the border again,
                # the center of the canvas has been reached
                if coord[coord_idx] + steps[step_idx] == borders[border_idx]:
                    break
        
        # if there are marked cells left at the end, paint them over
        self.cells = [[CellType.filled if cell == CellType.marked \
                                       else cell \
                                       for cell in row] \
                                       for row in self.cells]

class Geometry:
    def __init__(self, vertices: List[Vertice], triangles: List[Triangle]) -> None:
        self.vertices = vertices
        self.triangles = triangles
    
    def get_boundary(self, axis: str, kind: str) -> float:
        assert axis == 'x' or axis == 'y' or axis == 'z'
        assert kind == 'min' or kind == 'max'
        
        axis_values = {'x': 0, 'y': 1, 'z': 2}
        kind_values = {'min': min, 'max': max}
        
        return kind_values[kind]([v[axis_values[axis]] \
                                  for v in self.vertices])
    
    def prepare(self, factor: float) -> None:
        min_x = self.get_boundary('x', 'min')
        min_y = self.get_boundary('y', 'min')
        min_z = self.get_boundary('z', 'min')
        
        max_x = self.get_boundary('x', 'max')
        max_y = self.get_boundary('y', 'max')
        max_z = self.get_boundary('z', 'max')
        
        maximals = (max_x, max_y, max_z)
        minimals = (min_x, min_y, min_z)
        
        coef = factor/max(map(lambda t: t[0]-t[1],
                              zip(maximals, minimals)))
        
        self.vertices = [tuple(map(lambda t: (t[0]-t[1])*coef + 0.5,
                                   zip(vert, minimals))) \
                         for vert in self.vertices]
        
class OBJGeometry(Geometry):
    def __init__(self, filename: str) -> None:
        data = [s.split() \
                for s in open(filename).readlines()]
        
        vertices = []
        triangles = []
        
        for raw_vertice in [s for s in data if s[0] == 'v']:
            assert len(raw_vertice[1:]) == 3
            vertices += [tuple(map(float, raw_vertice[1:]))]
        
        for raw_face in [s for s in data if s[0] == 'f']:
            assert len(raw_face[1:]) == 3
            triangles += [tuple([int(idx.split(sep = '/')[0])-1 \
                          for idx in raw_face[1:]])]
        
        super().__init__(vertices, triangles)
        
class Slicer:
    def __init__(self, geometry: Geometry, dots_per_side: int) -> None:
        assert dots_per_side > 1
        
        self.dots_per_side = dots_per_side
        self.geometry = geometry
        self.geometry.prepare(float(self.dots_per_side-1))

    def slice(self, thickness: float) -> List[Canvas]:
        layers_amount = int(self.geometry.get_boundary('z', 'max')/thickness)
        layers = []
        
        epsilon = 1e-6
        
        for i in range(layers_amount):
            layer_canvas = Canvas(self.dots_per_side,
                                  self.dots_per_side)
            
            current_z = (i + 0.5) * thickness

            for tri in self.geometry.triangles:
                temporary_canvas = Canvas(self.dots_per_side,
                                          self.dots_per_side)
                lines = []
                
                upper_points = []
                lower_points = []
                layer_points = []
                
                triangle = tuple([self.geometry.vertices[v] for v in tri])
                
                for point in triangle:
                    if abs(point[2] - current_z) < epsilon:
                        layer_points += [point]
                    elif point[2] > current_z:
                        upper_points += [point]
                    else:
                        lower_points += [point]
                
                # the triangle does not intersect the plane,
                # there is nothing to do here
                if len(upper_points) == 3 or \
                   len(lower_points) == 3 or \
                   len(layer_points) != 3:
                    continue
                
                elif len(layer_points) == 3:
                    for i in range(len(layer_points)-1):
                        for j in range(i+1, len(layer_points)):
                            lines += [(layer_points[i][:-1],
                                       layer_points[j][:-1])]

                
                # the most difficult case is when a triangle intersects a plane,
                # requiring calculation of the coordinates
                # of the intersection points
                else:
                    start_point = None
                    final_points = []
                    
                    if len(upper_points) == 2:
                        final_points = upper_points
                        start_point = lower_points[0]
                    else:
                        final_points = lower_points
                        start_point = upper_points[0]
                    
                    # we will calculate the parameters for parametric equations
                    # of lines 'start -> final[0]' and 'start -> final[1]',
                    # for which the z coordinate is equal to the
                    # height of the plane (current_z)
                    params = [(current_z - start_point[2])/(p[2] - start_point[2]) \
                              for p in final_points]
                    
                    lines += [tuple([(start_point[0]*(1-params[i]) + \
                                      final_points[i][0]*params[i],
                                      start_point[1]*(1-params[i]) + \
                                      final_points[i][1]*params[i]) \
                                      for i in range(2)])]
            
                for line in lines:
                    temporary_canvas.draw_line(line)
                    temporary_canvas.fill_holes()
                layer_canvas.merge(temporary_canvas)
                
            layers += [layer_canvas]
        
        return layers
