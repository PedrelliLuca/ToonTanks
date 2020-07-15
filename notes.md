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

### Lesson 150 - Adding health functionality

Clamp(x, a, b) makes sure that the value x (obtained by a calculation of some sort) doesn't get beyond the boundaries of the interval [a, b]; if x<a -> x is made equal to a, if x>b then x is made equal to b.

We're not passing anything to our owning actor to let them know that they've died. That's purely due to the complexity of our game: we know that we only have two pawns classes in the game and both of them are kind of relevant to the game flow via the game mode class.

So, rather than passing off multiple messages, one to the game mode to let them know that something's died and then one to the owning pawn also let it know that it died, we're just gonna pass this to the game mode.

The game mode can then pass informations to other classes if needed.

#### Damage flow
Projectile class executes ApplyDamage on OtherActor -> this fires OnTakeAnyDamage on OtherActor, i.e. the pawn that was hit -> thanks to dynamic binding we fire TakeDamage instead!

DON'T FORGET TO ATTACH THE COMPONENT TO YOUR PAWNS

### Lesson 151 - GameMode ActorDied

`GetAllActorsOfClass` function returns us an array of all the actor instances of a specified subclass that are in the level.

Functions that require a world context object `const UObject * WorldContextObject`, like `GetAllActorsOfClass` or `GetPlayerPawn` you don't need to pass `GetWorld()`: any object that is the world you're interested in will do just fine! Therefore, often `this` is a valid choice!

### Lesson 152 + 153 - Widget blueprints

Let's create the widget that appears at the beginning of our game

Content browser: Right click -> User interface -> widget blueprint

Hierarchy: controls order in which things are displayed in the canvas.

When you select a widget component, like a text box, if you want to implement a logic for it (i.e. use it in the graph section) **don't forget to tick the "IsVariable" checkbox** in the topright of the details panel.

Careful at constructors in unreal: if you place there reference to things you might easily get crashes, because constructors are executed before BeginPlay() and things do not exist in the world before BeginPlay().

### Lesson 154 - GameMode blueprint

**Through the gamemode blueprint**, we'll be creating and displaying our start and end widgets to the screen, which will encapsulate the flow of gameplay that we currently have going.

To override the functions that we have in our C++ class, we go over to the function section, highlight the function text to get the override option, and then press that button to dropdown the available functions.

The GameStart and GameOver BP functions, which are called on the C++ side (TankGameModeBase.cpp) will create our widgets!

There is a create widget node, but this is not enough to show the widgets, this node just creates them! To show them we need another node, AddToViewport. But, **BEFORE DOING THAT, we need to set the values our widgets need!**

Flow: Create widgets -> set variables -> add to viewport.

### Lesson 155 - Dynamic variable update debug

Never, ever, but dynamic variable updates like `Health = DefaultHealth` in the class constructor!! Do them in BeginPlay. Otherwise, if you try changing values from BPs, they won't affect the instances on the map.

### Lesson 156 - Player controllers

Player controllers are the interface between the player or A.I. and that controlled pawn.

We won't be going into A.I. controllers in this section as our A.I. is very simplistic and doesn't really need a controller to handle communication between things like the AI Blackboard system.

We will, however, be seeing some of the things that we can get our player controller to handle.

**The player controller is the interface that directly receives the input notifications when something is pressed and also what is used to enable or disable the player input.**

For example: player pawn class can call its owning player controller and enable or disable the input during a cutscene.

We'll use the playercontroller class to enable/disable input and to show the mouse cursor.

Bug: if an input is still pressed when the game ends it stays.

### lesson 157 - PawnTank destruction

Create a new variable `bAlive` and a getter in the tank header to communicate to the turrets when the player is dead. At tank destruction:

1. `SetActorHiddenInGame(true)`

2. `SetActorTickEnabled(false)` to disable inputs
3. Set `bAlive = false`.

When the turret `CheckFireCondition`, if the player is dead turrets stop firing.

THIS CONCLUDES OUR GAME LOOP :) Now we just need to play with the effects

### Lesson 158 + 159 - Spawning particles

Let's implement particle effects! effect class: `UParticleSystem`. This is not a component.

Let's also add the UHealthComponent to the class and make the constructor create one by default, since it's something that every tank / turret needs.

We can also implement particles as components, `UParticleSystemComponent` which is what we want to do for our particle trail effect. The component keeps spawning particles over its life based on an updating position of the projectile. It is a scene component, so it needs to be attached to something, we'll attach it to the rootcomponent of the projectile (i.e. its root component).

### Lesson 160 - Playing sound effects

USoundBase for the effect; `USoundBase`. Launch and Hit sounds on projectiles, Death sound on PawnBase.
