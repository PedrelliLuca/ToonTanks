# Project Notes

### Lesson 135

All the attributes with the U prefix will need the `UPROPERTY()` macro above them

### Lesson 136

We derived blueprint classes from our base class. Let's examine them:

Turret class has everything we need to make the enemy turret function.
- Visual representation: the two static mesh components
- Collision setup: The capsule component
- Point to spawn projectile from: scene component

For the tank, since it's the pawn controlled by the player, we will add a camera component and a spring arm component, which gives us extra control on the camera.
