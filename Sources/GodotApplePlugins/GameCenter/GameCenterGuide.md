# Using Apple's GameKit APIs with Godot

This is a quick guide on using the APIs in this Godot addon to access
Apple's GameKit APIs.  For an overview of what you can do with
GameKit, check [Apple's GameKit
Documentation](https://developer.apple.com/documentation/gamekit/)

One of the design choices in this binding has been to surface the same
class names that Apple uses for their own data types to simplify
looking things up and finding resources online.  The method names on
the other hand reflect the Godot naming scheme.

So instead of calling `loadPhoto` on GKPlayer, you would use the
`load_photo` method.  And instead of the property `gamePlayerID`, you
would access `game_player_id`.

# Table of Contents

* [Installation]
* [Players]

## Installation

### Installing in your project

Make sure that you have added the directory containing the
"GodotApplePlugins" to your project, it should contain both a
`godot_apple_plugins.gdextension` and a `bin` directory with the
native libraries that you will use.

The APIs have been exposed to both MacOS and iOS, so you can iterate
quickly on your projects.

### Entitlements

For your sofwtare to be able to use the GameKit APIs, you will need
your Godot engine to have the `com.apple.developer.game-center`
entitlements.  The easiest way to do this is to use Xcode to add the
entitlement to your iOS project.

See the file [Entitlements](Entitlements.md) for additional directions
- without this, calling the APIs won't do much.

## Authentication

Create an instance of GameCenterManager, and then you can connect to
the `authentication_error` and `authentication_result` signals to
track the authentication state.

Then call the `authenticate()` method to trigger the authentication:

```gdscript
var gameCenter: GameCenterManager

func _ready() -> void:
	gameCenter = GameCenterManager.new()

	gameCenter.authentication_error.connect(func(error: String) -> void:
		print("Received error %s" % error)
	)
	gameCenter.authentication_result.connect(func(status: bool) -> void:
		print("Authentication updated, status: %s" % status
	)
```

The `local_player` is a special version of `GKPlayer` with properties
that track the local player state.

## Players

### Fetch the Local Player

From the GameCenterManager, you can call `local_player`, this returns
a `GKLocalPlayer`, which is a subclass of `GKPlayer` and represents
the player using your game.

```gdscript
var local: GKLocalPlayer

func _ready() -> void:
	gameCenter = GameCenterManager.new()
	local = gameCenter.local_player
	print("ONREADY: local, is auth: %s" % local.is_authenticated)
	print("ONREADY: local, player ID: %s" % local.game_player_id)
```

There are a number of interesting properties in local_player that you
might want to use in your game like `is_authenticated`, `is_underage`,
`is_multiplayer_gaming_restricted` and so on.

### GKPlayer

* [GKPlayer]((https://developer.apple.com/documentation/gamekit/gkplayer)

This is the base class for a player, either the local one or friends
and contains properties and methods that are common to both

Apple Documentation:

* [GKLocalPlayer](https://developer.apple.com/documentation/gamekit/gklocalplayer)

### Loading a Player Photo


```gdscript
# Here, we put the image inside an existing TextureRect, named $texture_rect:
local_player.load_photo(true, func(image: Image, error: Variant)->void:
	if error == null:
		$texture_rect.texture = ImageTexture.create_from_image(image)
)
```

### Friends

```gdscript
# Loads the local player's friends list if the local player and their friends grant access.
local_player.load_friends(func(friends: Array[GKPlayer], error: Variant)->void:
	if error:
		print(error)
	else:
		for friend in friends:
			print(friend.displayName)
)

# Loads players to whom the local player can issue a challenge.
local_player.local.load_challengeable_friends(func(friends: Array[GKPlayer], error: Variant)->void:
    if error:
        print(error)
    else:
        for friend in friends:
            print(friend.displayName)
)

# Loads players from the friends list or players that recently participated in a game with the local player.
local.load_recent_friends(func(friends: Array[GKPlayer], error: Variant)->void:
    if error:
        print(error)
    else:
        for friend in friends:
            print(friend.displayName)
)
```

### FetchItemsForIdentityVerificationSignature

* [Apple Documentation](https://developer.apple.com/documentation/gamekit/gklocalplayer/3516283-fetchitems)

```
local.fetch_items_for_identity_verification_signature(func(values: Dictionary, error: Variant)->void:
    if error:
        print(error)
    else:
        print("Identity dictionary")
        print(values)
)
```

## Achievements

* [GKAchievement](https://developer.apple.com/documentation/gamekit/gkachievement)
* [GKAchievementDescription](https://developer.apple.com/documentation/gamekit/gkachievementdescription)

### List all achievements

Note: This only returns achievements with progress that the player has reported. Use GKAchievementDescription for a list of all available achievements.

```gdscript
GKAchievement.load_achievements(func(achievements: Array[GKAchievement], error: Variant)->void:
    if error:
        print("Load Achivement error %s" % error)
    else:
        for achievement in achievements:
            print("Achievement: %s" % achievement.identifier)
)
```

### List Descriptions

```gdscript
GKAchievementDescription.load_achievement_descriptions(func(adescs: Array[GKAchievementDescription], error: Variant)->void:
    if error:
        print("Load AchivementDescription error %s" % error)
    else:
        for adesc in adescs:
            print("Achievement Description ID: %s" % adesc.identifier)
            print("    Unachieved: %s" % adesc.unachieved_description)
            print("    Achieved: %s" % adesc.achieved_description)
)
```

### Load Achievement Description Image

```gdscript
adesc.load_image(func(image: Image, error: Variant)->void:
    if error == null:
        $texture_rect.texture = ImageTexture.create_from_image(image)
    else:
        print("Error loading achievement image %s" % error)                            
```

### Report Progress

```gdscript
var id = "a001"
var percentage = 100

GKAchievement.load_achievements(func(achievements: Array[GKAchievement], error: Variant)->void:
    if error:
        print("Load Achivement error %s" % error)
    else:
        for achievement in achievements:
            if achievement.identifier == id:
                if not achievement.is_completed:
                    achievement.percent_complete = percentage
                    achievement.show_completion_banner = true
                GKAchievement.report_achivement([achievement], func(error: Variant)->void: 
                    if error:
                        print("Error submitting achievement")
                    else:
                        print("Success!")
                )
```

### Reset All Achievements

```gdscript
GKAchievement.reset_achivements(func(error: Variant)->void:
    if error:
        print("Error resetting" % error)
    else:
        print("Success")
)
```

# Realtime Matchmaking

* [GKMatch](https://developer.apple.com/documentation/gamekit/gkmatch)
* [GKMatchRequest](https://developer.apple.com/documentation/gamekit/gkmatchrequest)


## Events

You can use the convenience request_match method after configuring your request,
and on your callback setup the gameMatch to track the various states of the match,
like this:

```gdscript
var req = GKMatchRequest.new()
req.max_players = 2
req.min_players = 1
req.invite_message = "Join me in a quest to fun"
GKMatchmakerViewController.request_match(req, func(gameMatch: GKMatch, error: Variant)->void:
    if error:
        print("Could nto request a match %s" % error)
    else:
        print("Got a match!")

        gameMatch.data_received.connect(func (data: PackedByteArray, fromPlayer: GKPlayer)->void:
            print("received data from Player")
        )
        gameMatch.data_received_for_recipient_from_player.connect(func(data: PackedByteArray, forRecipient: GKPlayer, fromRemotePlayer: GKPlayer)->void: 
            print("Received data from a player to another player")
        )
        gameMatch.did_fail_with_error.connect(func(error: String)->void:
            print("match failed with %s" % error)
        )
        gameMatch.should_reinvite_disconnected_player = (func(player: GKPlayer)->bool:
            # We always reinvite
            return true
        )
        gameMatch.player_changed.connect(func(player: GKPlayer, connected: bool)->void: 
            print("Status of player changed to %s" % connected)
        )
)
```

## Disconnect

```gdscript
gameMatch.disconnect()
```

## Send to all

```gdscript
    var data = "How do you do fellow kids".to_utf8_buffer()
    gameMatch.send_data_to_all_players(data, GKMatch.SendDataMode.reliable)
```

## Send to Players

```gdscript
                
gameMatch.send(array, [firstPlayer, secondPlayer], GKMatch.SendDataMode.reliable)

```

# Leaderboards

## Report Score

```gdscript
GKLeaderboard.load_leaderboards(["MyLeaderboard"], func(leaderboards: Array [GKLeaderboard], error: Variant)->void:
    var score = 100
    var context = 0
    
    leaderboards[0].submit_score(score, context, local, func(error: Variant)->void:
        if error:
            print("Error submitting leadeboard %s" % error)
    )
)
```

## Load Leaderboards

```
# Loads all leaderboards
GKLeaderboard.load_leaderboards([], func(leaderboards: Array [GKLeaderboard], error: Variant)->void:
    print("Got %s" % leaderboards)
)

# Load specific ones
GKLeaderboard.load_leaderboards(["My leaderboard"], func(leaderboards: Array [GKLeaderboard], error: Variant)->void:
    print("Got %s" % leaderboards)
)

```

## Load Scores

