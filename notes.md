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

`RotateTurret()` and `Fire()` will carry out the same functionalities in both the tank and turret pawn classed, the only difference is when they get called. For this reason, these function won't be overridden in PawnTank and PawnTurret.

`Fire()` is called:
- By turrets's `CheckFireCondition()` every X seconds if fire conditions are met.
- By tank when the LeftMouseButton is pressent (IE_Pressed PlayerInputComponent->BindAction)

`RotateTurret()` is called:
- Every tick by the turret. We pass the player pawn location to it.
- Every tick by the tank. We pass the mouse position to it.

`HandleDestruction()` however needs to do different things in Turret and Tank, therefore we need to make it virtual and override it.
- On turrets we can safely call AActor::Destroy()
- On the player controlled class calling Destroy() leads to janky results (the camera attached to the object gets destroyed and we jump to a default camera in the world). For this reason, on player controlled class it's better to:
  - Hide visual components
  - Disable movement and inputs

  This way we keep the camera!

### Lesson 145 - Creating actor classes

For Projectiles, we're gonna use actor classes because they need less functionalities than our tank and turrets (which are pawns).

Components:
- Projectile movement component: standard component from unreal that provides constant movement to the class that it's on, including things like gravity, homing, bouncing etc..
- Static mesh
- We want to assign a damage type -> we're gonna implement a template class `TSubclassOf<UDamageType>`
- floats: Speed and damage.

Why `TSubclassOf<UDamageType>` and not simply `UDamageType*`? It's down to the options and type security that this provides: we're unable to just declare a UDamageType and create a pointer to it and then name it damage type like we would have done in the past as this will provide compile errors.

An alternative would be to declare a `UClass*`, which is much broader than any pointer to component we created so far! We could then make it point to a UDamageType instance and this would work with no compilation errors. Problem: other developers, unfamiliar with our work and intentions, might assign any other derived class to this pointer (and there is a ton of them, it's very easy to make mistakes), not just a UDamageType, leading to errors. Therefore, using such a broad pointer is considered a bad practice.

`TSubclassOf<UDamageType>` will ensure that the end user will only be able to select something that is either the base UDamageType or a subclass of that.

`UProjectileMovementComponent` instances don't have a transform, so they don't belong in the scene hierarchy or inherit any transforms from their parent classes. Therefore, we don't need to do the usual process of attaching these to anything.

### Lesson 146 - Spawning actors

If, once spawned, the bullet is frozen, that's because as soon as the projectile instance is being spawned, it's overlapping with the pawn classes and the projectile movement component will then just stop its movement.

To avoid this, just move the scene component that handles its spawn farther away from the turret and you'll be fine!

At the moment, the projectiles keep hanging around after being fired, we need to fix this:

### Lesson 147 - Using dynamic delegates

We'll be seeing how we can implement dynamic delegates to our projectile class.

**Dynamic delegates allow us to bind a custom function to be fired whenever a default function on the class is run.**

The ProjectileMesh is the component, and we want to bind `OnComponentHit` to it. This is done via `AddDynamic` + the function that we want to run each time `OnComponentHit` is called.

When we create our custom function or the the custom version of this, that we'll be using to run our hit functionality and bind to this delegates, this kind of works in a similar way to overwriting a parent class function in that **we still need to provide all of the function arguments**.

The reason that we're using `GetInstigatorController()` on the owner is that controllers kind of work within reference to the owning pawn, whether that be a player controller, such as one possessed by a human player, or an A.I. controller, such as something which will be possessing an instance of AI-Pawn. Basically, the bullet is asking "what was controlling the thing that spawned me?".

To recap: the delegate `AProjectileBase::OnHit` is called every time `StaticMesh` `OnComponentHit` function is called, and we're able to access all of the argument `OnComponentHit` would take.

`AProjectileBase::OnHit` is basically a custom version, an override, of `OnComponentHit`.

### Lesson 148 - Creating GameMode classes

Game Mode Base is one of the standard options when creating a c++ class.

A game mode dictates the rules of gameplay (its rules, scoring, which pawn is controlled by the player, the game state etc).

`BeginPlay()` is the usual standard unreal virtual void function. It will collect all relevant infos when the game starts (for example, the # of turrets).

`UFUNCTION(BlueprintImplementableEvent)` will allow us to handle all of the functionality for these two functions in the derived blueprint class. Certain things like setting timers, creating widgets etc are a lot more efficient and time friendly inside of blueprints than in the private section.

`BlueprintImplementableEvent` don't need any C++ implementations. In fact, you'll get errors if you give them one. Why do we define them in the code then? So that we can call them from other functions!

So the way that Unreal works is that every game that we create will need a game mode class to control what is spawned in the level after the level is initialized. If you don't provide one like we haven't so far, then the project will create a default game mode class prefilled with some default classes that we can't override.

By setting the default pawn class to be BP_PawnTank we can delete the tank that was previously in the map, one will be automatically created at PlayerStart when the game begins and we'll be in control!

### Lesson 149 - Creating a health component

We'll be creating our health component, which will be a flexible class, allowing us to record health and damage for any actor or pawn in the world that we place this on. You could technically assign health to static actors too (e.g. barrels), but we'll only assign health to our pawns.

#### Actor components VS scene components

Both can be placed on actors, however
- A scene component **has a transform**, i.e. it has a place in the world and can be moved around in the world when attached to other scene components
- An actor component can only be attached to an actor class (or its childrens like pawns etc) and not to other components since it doesn't have a transform and, therefore, a representation/place in the world.

hierarchy: SceneComponent is a child of ActorComponent

Health doesn't need to be placed in the world and other components don't need health -> our health component can be an actor component!
