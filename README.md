# Assignment 2: Spirographs
This interactive graphic simulates a spirograph, visualizing how the pattern changes with the cirlce size.

### Program Design
The spirograph meshes are generated in the Model via custom mesh objects. Three `CircleOutline.h` meshes are pushed to the view, representing the red outer circle, orange inner circle, and green seed. Additionally, a `CurveOutline.h` mesh representing the path of the rotating seed is calculated and pushed to the View.

In the View, the pushed meshes from the Model are drawn and transformed accordingly via helper methods in `display()`. The `onkey` actions handled in the Controller are accounted for in the View. For instance, when the size of the inner circle changes, not only does the value of the inner circle in the Model update, but the mesh in the View gets refreshed as well.

### Program Features
- Decrease the inner circle radius by pressing the lowercase 'i' key
- Increase the inner circle radius by pressing the uppercase 'I' key
- Toggle the visibility of the seed curve off and on by pressing the uppercase 'C' key
- Circle outline generation via `CircleOutline.h`
- Seed curve generation via `CurveOutline.h`
- Draw meshes in the View via `View::drawOuterCircle()`, `View::drawInnerCircle()`, etc.
- Update inner radius on key press via `Model::updateInnerRadius()`
- Refresh meshes on key press via `View::updateInnerCircle()` and `View::updateCurve()`

To run the program after make use `./Spirograph`

<img src="Spirograph.gif" width="400" height="400" />
