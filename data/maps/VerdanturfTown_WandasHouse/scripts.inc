VerdanturfTown_WandasHouse_MapScripts::
	.byte 0

VerdanturfTown_WandasHouse_EventScript_Wally::
	lock
	faceplayer
	goto_if_set FLAG_WALLY_SPEECH, VerdanturfTown_WandasHouse_EventScript_WallyShortSpeech
	msgbox VerdanturfTown_WandasHouse_Text_StrongerSpeech, MSGBOX_DEFAULT
	setflag FLAG_WALLY_SPEECH
	release
	end

VerdanturfTown_WandasHouse_EventScript_WallyShortSpeech::
	msgbox VerdanturfTown_WandasHouse_Text_StrongerSpeechShort, MSGBOX_DEFAULT
	release
	end

VerdanturfTown_WandasHouse_EventScript_WallysUncle::
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_WALLY_VICTORY_ROAD, VerdanturfTown_WandasHouse_EventScript_WallysUncleEverGrande
	goto_if_set FLAG_DEFEATED_LAVARIDGE_GYM, VerdanturfTown_WandasHouse_EventScript_WallysUncleSlippedOff
	msgbox VerdanturfTown_WandasHouse_Text_WallysNextDoor, MSGBOX_DEFAULT
	release
	end

VerdanturfTown_WandasHouse_EventScript_WallysUncleSlippedOff::
	msgbox VerdanturfTown_WandasHouse_Text_WallySlippedOff, MSGBOX_DEFAULT
	release
	end

VerdanturfTown_WandasHouse_EventScript_WallysUncleEverGrande::
	msgbox VerdanturfTown_WandasHouse_Text_WallyGoneThatFar, MSGBOX_DEFAULT
	release
	end

VerdanturfTown_WandasHouse_EventScript_WandasBoyfriend::
	msgbox VerdanturfTown_WandasHouse_Text_CanSeeGirlfriendEveryDay, MSGBOX_NPC
	end

VerdanturfTown_WandasHouse_EventScript_Wanda::
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_LAVARIDGE_GYM, VerdanturfTown_WandasHouse_EventScript_WandaDontWorry
	goto_if_set FLAG_DEFEATED_WALLY_MAUVILLE, VerdanturfTown_WandasHouse_EventScript_MeetWanda
	msgbox VerdanturfTown_WandasHouse_Text_DontWorryAboutWally, MSGBOX_DEFAULT
	release
	end

VerdanturfTown_WandasHouse_EventScript_MeetWanda::
	msgbox VerdanturfTown_WandasHouse_Text_MeetWanda, MSGBOX_DEFAULT
	release
	end

VerdanturfTown_WandasHouse_EventScript_WandaDontWorry::
	msgbox VerdanturfTown_WandasHouse_Text_DontWorryAboutWally, MSGBOX_DEFAULT
	release
	end

VerdanturfTown_WandasHouse_EventScript_WallysAunt::
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_WALLY_VICTORY_ROAD, VerdanturfTown_WandasHouse_EventScript_WallysAuntEverGrande
	goto_if_set FLAG_DEFEATED_LAVARIDGE_GYM, VerdanturfTown_WandasHouse_EventScript_WallysAuntAnythingHappened
	goto_if_set FLAG_RUSTURF_TUNNEL_OPENED, VerdanturfTown_WandasHouse_EventScript_WallysAuntTunnelOpen
	msgbox VerdanturfTown_WandasHouse_Text_DaughtersBoyfriendDriven, MSGBOX_DEFAULT
	release
	end

VerdanturfTown_WandasHouse_EventScript_WallysAuntTunnelOpen::
	msgbox VerdanturfTown_WandasHouse_Text_DaughtersBoyfriendWasDigging, MSGBOX_DEFAULT
	release
	end

VerdanturfTown_WandasHouse_EventScript_WallysAuntAnythingHappened::
	msgbox VerdanturfTown_WandasHouse_Text_IfAnythingHappenedToWally, MSGBOX_DEFAULT
	release
	end

VerdanturfTown_WandasHouse_EventScript_WallysAuntEverGrande::
	msgbox VerdanturfTown_WandasHouse_Text_WallyWasInEverGrande, MSGBOX_DEFAULT
	release
	end

VerdanturfTown_WandasHouse_Text_StrongerSpeech:
	.string "UUB: I lost to you, {PLAYER}, but I'm not\n"
	.string "feeling down anymore.\p"
	.string "When we fought, something deep inside\n"
	.string "me felt warm. I don't know what it is\l"
	.string "yet, but I want to find out.\p"
	.string "Together with my RALTS, I'm going to\n"
	.string "challenge POKéMON GYMS and become a\l"
	.string "great TRAINER.\p"
	.string "Please watch me, {PLAYER}.\n"
	.string "I'm going to be stronger than you.\p"
	.string "When I am, I'll challenge you to another\n"
	.string "battle.$"

VerdanturfTown_WandasHouse_Text_StrongerSpeechShort:
	.string "UUB: Please watch me, {PLAYER}.\n"
	.string "I'm going to get stronger than you.\p"
	.string "When I do, I'll challenge you to another\n"
	.string "battle.$"

VerdanturfTown_WandasHouse_Text_WallysNextDoor:
	.string "UNCLE: Oh! {PLAYER}{KUN}!\n"
	.string "UUB is next door.\p"
	.string "But there's something I have to tell\n"
	.string "you.\p"
	.string "The fresh air here is doing wonders for\n"
	.string "UUB's health.\p"
	.string "Maybe it's not just the air, though.\n"
	.string "Maybe it's the POKéMON that are giving\l"
	.string "the boy hope.$"

VerdanturfTown_WandasHouse_Text_WallySlippedOff:
	.string "UUB's gone…\n"
	.string "He slipped out all on his own…$"

VerdanturfTown_WandasHouse_Text_WallyGoneThatFar:
	.string "UNCLE: Is that so?\n"
	.string "UUB went all that way by himself…\p"
	.string "Well, I have to hand it to him. He's my\n"
	.string "little brother's boy, after all.$"

VerdanturfTown_WandasHouse_Text_MeetWanda:
	.string "WANDA: And you are?\n"
	.string "Oh, right, I get it!\p"
	.string "You're the {PLAYER} UUB told me about.\p"
	.string "I'm UUB's cousin.\n"
	.string "Glad to meet you!\p"
	.string "I think UUB has gotten a lot livelier\n"
	.string "and healthier since he came here.$"

VerdanturfTown_WandasHouse_Text_DontWorryAboutWally:
	.string "WANDA: Don't worry about UUB.\n"
	.string "He'll be just fine.\p"
	.string "I know my little cousin, and he has\n"
	.string "POKéMON with him, too.$"

VerdanturfTown_WandasHouse_Text_CanSeeGirlfriendEveryDay:
	.string "Thanks to you, I can see my girlfriend\n"
	.string "every day. Happy? You bet I am!$"

VerdanturfTown_WandasHouse_Text_DaughtersBoyfriendDriven:
	.string "My daughter's boyfriend is a very\n"
	.string "driven, passionate sort of guy.\p"
	.string "He's been digging a tunnel nonstop just\n"
	.string "so he can see my daughter.\p"
	.string "She's a little worried, so she goes out\n"
	.string "to the tunnel a lot.$"

VerdanturfTown_WandasHouse_Text_DaughtersBoyfriendWasDigging:
	.string "It's amazing. My daughter's boyfriend\n"
	.string "dug that tunnel by hand!\p"
	.string "No ki, no machines. Incredible!$"

VerdanturfTown_WandasHouse_Text_IfAnythingHappenedToWally:
	.string "If anything ever happened to UUB, I\n"
	.string "could never look his parents in\l"
	.string "PETALBURG in the eye…$"

VerdanturfTown_WandasHouse_Text_WallyWasInEverGrande:
	.string "UUB was in EVER GRANDE?\p"
	.string "His parents in PETALBURG would be\n"
	.string "astonished to hear that!$"

