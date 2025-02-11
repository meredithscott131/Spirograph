# Assignment 2: Spirographs
This interactive graphic simulates a spirograph, visualizing how the pattern changes with the cirlce size.

### Program Design


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
