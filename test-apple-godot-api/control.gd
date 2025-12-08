extends Control

var gameCenter: GameCenterManager
var local: GKLocalPlayer
var auth_controller = ASAuthorizationController

func _ready() -> void:
	gameCenter = GameCenterManager.new()
	local = gameCenter.local_player
	print(Time.get_unix_time_from_system())
	#gameCenter.load_leaderboards(["BOARD_1", "BOARD_2"])
	print("ONREADY: game center, is %s" % gameCenter)
	print("ONREADY: local, is auth: %s" % local.is_authenticated)
	print("ONREADY: local, player ID: %s" % local.game_player_id)

	auth_controller = ASAuthorizationController.new()
	auth_controller.authorization_completed.connect(_on_authorization_completed)
	auth_controller.authorization_failed.connect(_on_authorization_failed)

func _on_button_pressed() -> void:
	# Request full name and email
	auth_controller.perform_apple_id_request(["full_name", "email"])

func _on_authorization_completed(credential):
	if credential is ASAuthorizationAppleIDCredential:
		print("User ID: ", credential.user)
		print("Email: ", credential.email)
		print("Full Name: ", credential.fullName)
	elif credential is ASPasswordCredential:
		print("User: ", credential.user)
		print("Password: ", credential.password)

func _on_authorization_failed(error_message):
	print("Authorization failed: ", error_message)

func _xon_button_pressed() -> void:
	var player = gameCenter.local_player
	print("Got %s" % player)
	print("Fetching the other object: %s" % player.is_authenticated)
	var demo = GKLeaderboard.new()
	
	gameCenter.authentication_error.connect(func(error: String) -> void:
		$auth_result.text = error
		)
	gameCenter.authentication_result.connect(func(status: bool) -> void:
		print("")
		if status:
			$auth_result.text = player.display_name
			$auth_state.text = "Authenticated"
			gameCenter.local_player.load_photo(true, func(image: Image, error: Variant)->void:
				if error == null:
					$texture_rect.texture = ImageTexture.create_from_image(image)
				else:
					print(error)
				)
				
			GKLeaderboard.load_leaderboards(["MyLeaderboard"], func(leaderboards: Array [GKLeaderboard], error: Variant)->void:
				var score = 100
				var context = 0
				
				leaderboards[0].submit_score(score, context, local, func(error: Variant)->void:
					if error:
						print("Error submitting leadeboard %s" % error)
				)
			)
			
			$auth_state.text = "Not Authenticated"
		)
	gameCenter.authenticate()

func _on_button_requestmatch_pressed() -> void:	
	if true:
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
				var array = "Hello".to_utf8_buffer()
				var first = local
				var second = local
				
				gameMatch.send_data_to_all_players(array, GKMatch.SendDataMode.reliable)
				
				gameMatch.send(array, [first, second], GKMatch.SendDataMode.reliable)
		)
		print("Not authenticated, authenticate first")
