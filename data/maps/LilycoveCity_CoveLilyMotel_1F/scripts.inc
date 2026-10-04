LilycoveCity_CoveLilyMotel_1F_MapScripts::
	.byte 0

LilycoveCity_CoveLilyMotel_1F_EventScript_MotelOwner::
	lockall
	goto_if_set FLAG_SYS_GAME_CLEAR, LilycoveCity_CoveLilyMotel_1F_EventScript_GameClear
	goto_if_set FLAG_BADGE07_GET, LilycoveCity_CoveLilyMotel_1F_EventScript_AquaHideoutBusted
	msgbox LilycoveCity_CoveLilyMotel_1F_Text_GuestsDoubledByMascot, MSGBOX_DEFAULT
	applywaitmovement LOCALID_MOTEL_OWNER, Common_Movement_FacePlayer
	msgbox LilycoveCity_CoveLilyMotel_1F_Text_NoGuestsWithTeamAqua, MSGBOX_DEFAULT
	closemessage
	applywaitmovement LOCALID_MOTEL_OWNER, Common_Movement_FaceOriginalDirection
	releaseall
	end

LilycoveCity_CoveLilyMotel_1F_EventScript_AquaHideoutBusted::
	msgbox LilycoveCity_CoveLilyMotel_1F_Text_MonFoundLostItem, MSGBOX_DEFAULT
	applywaitmovement LOCALID_MOTEL_OWNER, Common_Movement_FacePlayer
	msgbox LilycoveCity_CoveLilyMotel_1F_Text_HeardAquaHideoutBusted, MSGBOX_DEFAULT
	closemessage
	applywaitmovement LOCALID_MOTEL_OWNER, Common_Movement_FaceOriginalDirection
	releaseall
	end

LilycoveCity_CoveLilyMotel_1F_EventScript_GameClear::
	msgbox LilycoveCity_CoveLilyMotel_1F_Text_HouseSittingMonCaughtBurglar, MSGBOX_DEFAULT
	applywaitmovement LOCALID_MOTEL_OWNER, Common_Movement_FacePlayer
	msgbox LilycoveCity_CoveLilyMotel_1F_Text_BetterGetWorkingOnGuestsDinner, MSGBOX_DEFAULT
	closemessage
	applywaitmovement LOCALID_MOTEL_OWNER, Common_Movement_FaceOriginalDirection
	releaseall
	end

LilycoveCity_CoveLilyMotel_1F_EventScript_BlockingTV::
	lockall
	playse SE_PIN
	applywaitmovement LOCALID_MOTEL_OWNER, Common_Movement_ExclamationMark
	applywaitmovement LOCALID_MOTEL_OWNER, Common_Movement_Delay48
	applywaitmovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterDown
	msgbox LilycoveCity_CoveLilyMotel_1F_Text_CantSeeTheTV, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_PLAYER, LilycoveCity_CoveLilyMotel_1F_Movement_PlayerPushFromTV
	applymovement LOCALID_MOTEL_OWNER, LilycoveCity_CoveLilyMotel_1F_Movement_OwnerPushPlayer
	waitmovement 0
	applywaitmovement LOCALID_MOTEL_OWNER, LilycoveCity_CoveLilyMotel_1F_Movement_OwnerReturn
	release
	end

LilycoveCity_CoveLilyMotel_1F_Movement_PlayerPushFromTV:
	face_right
	lock_facing_direction
	walk_left
	unlock_facing_direction
	step_end

LilycoveCity_CoveLilyMotel_1F_Movement_OwnerPushPlayer:
	walk_up
	step_end

LilycoveCity_CoveLilyMotel_1F_Movement_OwnerReturn:
	face_down
	walk_down
	face_up
	step_end

LilycoveCity_CoveLilyMotel_1F_Text_GuestsDoubledByMascot:
	.string "Hm, so that inn doubled its guests by\n"
	.string "using POKéMON as an attraction?\p"
	.string "Well, maybe I should make a cute\n"
	.string "POKéMON our motel's mascot.\p"
	.string "I wonder if that would bring in more\n"
	.string "guests?$"

LilycoveCity_CoveLilyMotel_1F_Text_NoGuestsWithTeamAqua:
	.string "Oh, sorry, sorry!\n"
	.string "I got caught up watching TV!\p"
	.string "Since the RED RIBBON ARMY moved into\n"
	.string "town, the tourists have been staying\l"
	.string "away.$"

LilycoveCity_CoveLilyMotel_1F_Text_CantSeeTheTV:
	.string "Hey, down in front!\n"
	.string "I can't see the TV!$"

LilycoveCity_CoveLilyMotel_1F_Text_MonFoundLostItem:
	.string "Amazing! You mean a POKéMON found\n"
	.string "somebody's lost item?\p"
	.string "Now that's something. If we had a smart\n"
	.string "POKéMON like that…\p"
	.string "We could track down anything our\n"
	.string "guests left behind…$"

LilycoveCity_CoveLilyMotel_1F_Text_HeardAquaHideoutBusted:
	.string "Oh, sorry, sorry!\n"
	.string "I got caught up watching TV!\p"
	.string "I heard someone busted up the RED\n"
	.string "RIBBON ARMY's SEA BASE.\p"
	.string "Thanks to that, we just got a\n"
	.string "reservation from a big group.\p"
	.string "They're from a company called… uh…\n"
	.string "CAPSULE something…$"

LilycoveCity_CoveLilyMotel_1F_Text_HouseSittingMonCaughtBurglar:
	.string "Amazing! A house-sitting POKéMON\n"
	.string "caught a burglar?\p"
	.string "Now that's something. If we had a tough\n"
	.string "POKéMON as a guard…\p"
	.string "We could keep our guests much safer.$"

LilycoveCity_CoveLilyMotel_1F_Text_BetterGetWorkingOnGuestsDinner:
	.string "Oh, sorry, sorry!\n"
	.string "I got caught up watching TV.\p"
	.string "Oh, right. A big group of guests checked\n"
	.string "in a little while ago.\p"
	.string "They signed in as CAPSULE CORP., so I\n"
	.string "guess they build gadgets.\p"
	.string "Uh-oh, I'd better get started on their\n"
	.string "dinner! That crowd looks hungry!$"

