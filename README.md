# 2D-Graphics-Editor
Designing and implementing a menu driven 2D graphics editor in C

Problem Statement: Design and implement a menu-driven 2D Graphics Editor in C using a 2D character array as the drawing canvas. The canvas should initially be filled with the character _ (underscore), and graphical objects should be drawn using the character * (asterisk).

Write functions to draw a circle, rectangle, line, and triangle.
You should be able to
add objects to the picture,
delete objects from the picture, and
modify objects in the picture.
Use a 2d array of characters to store the picture.
Write a function to display the picture.


The project involves developing a terminal-based 2D Graphics Editor in C that utilizes a 2D character array as its primary drawing canvas. The grid is initially populated with underscores (_), while various geometric shapes—such as lines, rectangles, triangles, and circles—are rendered onto the canvas using asterisks (*). Through a menu-driven interface, users can dynamically add, modify, or delete these graphical objects to create custom text-based artwork. The underlying state of the drawing is maintained entirely within the character matrix and is rendered to the screen via a dedicated display function.
