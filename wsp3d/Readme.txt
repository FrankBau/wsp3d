Steiner points are to be inserted
- on the (interior of) the finite faces of all cells
- along the finite edges of all cells

traversal graph nodes consist of
- vertex nodes corresponding to the vertices of the triangulation 
- face nodes corresponding to the Steiner points on the (interior of) the finite faces of all cells
- edge nodes corresponding to the Steiner points along the finite edges of all cells

traversal graph edges:

- vertex nodes are adjacent to :
  - all face nodes of all incident finite faces 
  - neighbouring edge nodes of all incident edges 

- face nodes are adjacent to :
  - all other face nodes of the same face
  - all face nodes of the other finite faces of both incident cells
  - all edge nodes on the 3 edges of the face 
  - 3 vertex nodes of the 3 vertices of the face 
  
- edge nodes are adjacent to :
  - 1..2 neighbouring edge nodes on the same edge  (if present)
  - 1..2 neighbouring vertex nodes on the same edge (if present)
  - all face nodes of all incident finite faces 
