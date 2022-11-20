"""
This demonstrates cutting on a .stl mesh, which requires some pre-processing
because STL stores 3 vertices for each face (so vertices are duplicated).

The merge_close_vertices is a naive implementation of this pre-processing
that will probably take a long time on large meshes. libigl provides
a remove_duplicates function that should be better :
  https://github.com/libigl/libigl/blob/master/include/igl/remove_duplicates.h

Thanks to @superzanti for providing the model
"""
##
import os
import numpy as np
import numpy.linalg as la
import mayavi.mlab as mlab
import itertools
import utils

import meshcut
##


def load_stl(stl_fname):
    import stl
    m = stl.mesh.Mesh.from_file(stl_fname)

    # Flatten our vert array to Nx3 and generate corresponding faces array
    verts = m.vectors.reshape(-1, 3)
    faces = np.arange(len(verts)).reshape(-1, 3)

    verts, faces = meshcut.merge_close_vertices(verts, faces)
    return verts, faces

##


if __name__ == '__main__':
    ##
    example_fname = os.path.join('data', 'sphere.stl')
    verts, faces = load_stl(example_fname)

    mesh = meshcut.TriangleMesh(verts, faces)
    ##

    def show(plane):
        P = meshcut.cross_section_mesh(mesh, plane)
        colors = [
            (0, 1, 1),
            (1, 0, 1),
            (0, 0, 1)
        ]
        print("num contours : ", len(P))

        if True:
            utils.trimesh3d(mesh.verts, mesh.tris, color=(1, 1, 1),
                            opacity=0.5, representation='wireframe')
            utils.show_plane(plane.orig, plane.n, scale=1, color=(1, 0, 0),
                             opacity=0.5)

            for p, color in zip(P, itertools.cycle(colors)):
                p = np.array(p)
                mlab.plot3d(p[:, 0], p[:, 1], p[:, 2], tube_radius=None,
                            line_width=10.0, color=color)
        return P

    ##
    plane_orig = (0, 0.75, 0)
    plane_norm = (0, 1, 0)
    plane = meshcut.Plane(plane_orig, plane_norm)
    show(plane)
    mlab.show()
    ##
    # This will align the plane with some edges, so this is a good test
    # for vertices intersection handling
    plane_orig = (0.6, -0.12510000169277191162, 0)
    plane_norm = (0.3, 0.7, -0.2)
    plane_norm /= la.norm(plane_norm)

    plane = meshcut.Plane(plane_orig, plane_norm)
    show(plane)
    mlab.show()
