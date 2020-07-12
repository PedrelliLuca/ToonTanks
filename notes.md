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

### Lesson 139 - Pawn movement input

You need to change the collisions presets of the capsule components to "block all", if you want sweeping to work. Setting it to true in `AddActorLocalOffset` is not enough. Then, make sure the capsule component doesn't overlap the floor, or your tank won't move at all!

### Lesson 140 - Exposing variables

When you expose new varibles in the editor you need to close and reopen the project.

Camera movement lag and rotation lag on the springarm give a more dynamic movement.

Changes in the class blueprints reflect on all the instances, but you can change values on a single instance too.

### Lesson 141 - Pawn Turret

Why `CheckFireCondition` is on a timer?

We could put it on a tick event where the timer is always being reset as a kind of cool down. But as we don't need an update quite often for the time tracking to look believable, we can save a little bit of performance here and also have a little bit extra control over the timer update rate.

Binding the `FireRateTimerHandler` timer to the `CheckFireCondition` function:
```cpp
GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandler, this,
        &APawnTurret::CheckFireCondition, FireRate, true, false);
```
- Second-last parameter: looping. If true, the function is called every FireRate seconds instead of just once.

- Last parameter: initial delay. False -> the turret starts firing immediately.

We could have just created a function named Fire and have this update on tick function: when certain conditions are met, we can fire projectiles from the turret.

However the tank class will also need a fire functionality! Therefore this is going to be something that we can create in the base class.

So, for readability, we'll use this check fire condition and this will be responsible for checking that the desired condition has been met to allow the firing functionality.

So `CheckFireCondition` is going to help avoid it looking like a really dumb A.I. that just fires every so many seconds, regardless of whether the player is alive, whether it's in range, and it'll just make it look that little bit more reactive to what's happening in the game.

```cpp
void APawnTurret::CheckFireCondition()
{
    // If player == nullptr || player == Dead
    //    return;
    // If player IS in range
    //    FIRE!!;
}
```

### Lesson 142 - Find distance between

`(UGameplayStatics::GetPlayerPawn(this, 0)` finds the player0 pawn. `this` refers to the current world reference, 0 stands for "Player0". The return type is `APawn`, so we need to cast this to `APawnTank`.

In `CheckFireCondition()` we can now check if the player PawnTank exists We don't have anything to check if the player's PawnTank is still alive, we'll come back to that later.

`Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this, 0));` is an expensive instruction, which is why we do that only once in BeginPlay and assign it to a ptr instead of doing it in `CheckFireCondition()`.

Don't forget to go to blueprint -> class settings -> parent class -> PawnTurret otherwise your code won't run and the editor will believe that the turret is a PawnBase!!

### Lesson 143 - Virtual functions

Things both PawnTank and PawnTurret need to have:

- Fire: they both should be able to fire projectiles to destroy the opponent.
- RotateTurret: A way to rotate the turret with respect to their base.
- HandleDestruction: Health and a functionality to be destroyed / removed from game when the health goes below a certain threshold.

Therefore, we can create these functionalities in the PawnBase in the protected section.

`RotateTurret()` and `Fire()` will only be called, not overridden, they'll carry out the same functionalities in both the tank and turret pawn classed.

`HandleDestruction()` however will do different things in Turret and Tank.
- On turrets we can safely call AActor::Destroy()
- On the player controlled class calling Destroy() leads to janky results (the camera attached to the object gets destroyed and we jump to a default camera in the world). For this reason, on player controlled class it's better to:
  - Hide visual components
  - Disable movement and inputs

  This way we keep the
