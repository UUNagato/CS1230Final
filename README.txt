-------------------------------- Instructions --------------------------------

After building and running the program, a window will pop up, showing our
swimming giraffe on water. For our UI control, users are able to adjust values
for the level of gradients, coefficient of diffuse light and specular light,
shininess, as well as the rgb values of the diffuse color on the side bar of
the window.

------------------------------------------------------------------------------

------------------------------ Design Decision -------------------------------

The project can be roughly divided into three parts: giraffe and swimming ring,
water surface, and shadow.

For the giraffe and the swimming ring, we first wrote an obj importer to import
obj files into our program and implemented toon shader on them. For the toon 
shader, we applied Blinn-Phong lighting model and added gradient layering effect.
For contour drawing, we used an algorithm called backface culling to achieve
pencil-drawing-like contour line effect.

For the water part, we chose to use a quad to implement the water surface. We mix
a lighter blue and a darker blue tint to create a water surface with gradient, 
and then added Perlin noise onto the water to create the effect of waves on the
water surface. We cut off the noise at a certain value and applied a step function
to the noise to optimize the appearance of waves on the water surface. For the
flowing effect and the movement of water (animation), we used a built-in class
qtime to record the elapsed time since starting the program and make the uv change
according to this value. Lastly, we added distortion effect by applying another
noise to the water surface to further optimize the appearance of waves. Moreover,
we also make the water transparent to make it obvious that the giraffe is floating 
up and down on the water surface.

For the shadow part, we implemented a basic shadow mapping. We used bias to 
eliminate the shadow acne caused by insufficient accuracy of float-point numbers. 
Further, we used PCF for shadow anti-aliasing and sine of time for water shadow 
fluctuation.

There are no known bugs in our program.

------------------------------------------------------------------------------