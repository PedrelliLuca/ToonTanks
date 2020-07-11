# Project Notes

### Lesson 135 - Creating components

All the attributes with the U prefix will need the `UPROPERTY()` macro above them

### Lesson 136 - Deriving blueprint classes

We derived blueprint classes from our base class. Let's examine them:

Turret class has everything we need to make the enemy turret function.
- Visual representation: the two static mesh components
- Collision setup: The capsule component
- Point to spawn projectile from: scene component

For the tank, since it's the pawn controlled by the player, we will add a camera component and a spring arm component, which gives us extra control on the camera.

### Lesson 137 - Access modifiers

Access specifiers explained

For components we'll use:
- VisibleAnywhere (default + instance)
- BlueprintReadOnly (we want to see them in blueprint because they are visual components and blueprints allow us to take a look at them)
- Category = "Components", for tidiness
- meta = (AllowPrivateAccess = "true") because the compiler normally wouldn't allow private memebers like our components to be visible in the editor

Now we have the details of the components exposed in our blueprints

### Lesson 138 - Creating child C++ class

The new class won't appear in the editor until you next restart the project.

Let's make a PawnTank class and add a SpringArm and Camera components to it!
