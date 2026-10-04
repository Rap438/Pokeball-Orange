SSTidalRooms_MapScripts::
	.byte 0

SSTidalRooms_EventScript_SnatchGiver::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_TM_SNATCH, SSTidalRooms_EventScript_ExplainSnatch
	msgbox SSTidalRooms_Text_NotSuspiciousTakeThis, MSGBOX_DEFAULT
	giveitem ITEM_TM_SNATCH
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setflag FLAG_RECEIVED_TM_SNATCH
	msgbox SSTidalRooms_Text_ExplainSnatch, MSGBOX_DEFAULT
	release
	end

SSTidalRooms_EventScript_ExplainSnatch::
	msgbox SSTidalRooms_Text_ExplainSnatch, MSGBOX_DEFAULT
	release
	end

SSTidalRooms_EventScript_Bed::
	lockall
	msgbox SSTidalRooms_Text_TakeRestOnBed, MSGBOX_DEFAULT
	closemessage
	call Common_EventScript_OutOfCenterPartyHeal
	call SSTidalRooms_EventScript_ProgessCruiseAfterBed
	releaseall
	end

SSTidalRooms_EventScript_Colton::
	trainerbattle_single TRAINER_COLTON, SSTidalRooms_Text_ColtonIntro, SSTidalRooms_Text_ColtonDefeat
	msgbox SSTidalRooms_Text_ColtonPostBattle, MSGBOX_AUTOCLOSE
	end

SSTidalRooms_EventScript_Micah::
	trainerbattle_single TRAINER_MICAH, SSTidalRooms_Text_MicahIntro, SSTidalRooms_Text_MicahDefeat
	msgbox SSTidalRooms_Text_MicahPostBattle, MSGBOX_AUTOCLOSE
	end

SSTidalRooms_EventScript_Thomas::
	trainerbattle_single TRAINER_THOMAS, SSTidalRooms_Text_ThomasIntro, SSTidalRooms_Text_ThomasDefeat
	msgbox SSTidalRooms_Text_ThomasPostBattle, MSGBOX_AUTOCLOSE
	end

SSTidalRooms_EventScript_Jed::
	trainerbattle_double TRAINER_LEA_AND_JED, SSTidalRooms_Text_JedIntro, SSTidalRooms_Text_JedDefeat, SSTidalRooms_Text_JedNotEnoughMons
	msgbox SSTidalRooms_Text_JedPostBattle, MSGBOX_AUTOCLOSE
	end

SSTidalRooms_EventScript_Lea::
	trainerbattle_double TRAINER_LEA_AND_JED, SSTidalRooms_Text_LeaIntro, SSTidalRooms_Text_LeaDefeat, SSTidalRooms_Text_LeaNotEnoughMons
	msgbox SSTidalRooms_Text_LeaPostBattle, MSGBOX_AUTOCLOSE
	end

SSTidalRooms_EventScript_Garret::
	trainerbattle_single TRAINER_GARRET, SSTidalRooms_Text_GarretIntro, SSTidalRooms_Text_GarretDefeat
	msgbox SSTidalRooms_Text_GarretPostBattle, MSGBOX_AUTOCLOSE
	end

SSTidalRooms_EventScript_Naomi::
	trainerbattle_single TRAINER_NAOMI, SSTidalRooms_Text_NaomiIntro, SSTidalRooms_Text_NaomiDefeat
	msgbox SSTidalRooms_Text_NaomiPostBattle, MSGBOX_AUTOCLOSE
	end

SSTidalRooms_Text_TakeRestOnBed:
	.string "There's a bed…\p"
	.string "Let's take a rest.$"

SSTidalRooms_Text_ColtonIntro:
	.string "I often sail to LILYCOVE CITY.\p"
	.string "I do enjoy attending CONTESTS, you see.$"

SSTidalRooms_Text_ColtonDefeat:
	.string "That was an enjoyable match!$"

SSTidalRooms_Text_ColtonPostBattle:
	.string "I get so excited imagining what kinds\n"
	.string "of POKéMON I'll see at the next\l"
	.string "CONTEST. The anticipation thrills me!$"

SSTidalRooms_Text_MicahIntro:
	.string "Are your friends strong?$"

SSTidalRooms_Text_MicahDefeat:
	.string "Your friends are indeed strong.$"

SSTidalRooms_Text_MicahPostBattle:
	.string "Friends needn't be human.\p"
	.string "To me, POKéMON are treasured friends!$"

SSTidalRooms_Text_ThomasIntro:
	.string "Child…\p"
	.string "Did you knock before entering?$"

SSTidalRooms_Text_ThomasDefeat:
	.string "A loss must be accepted without haste\n"
	.string "or panic.$"

SSTidalRooms_Text_ThomasPostBattle:
	.string "To stay unruffled in any situation is\n"
	.string "the GENTLEMAN's code of conduct.$"

SSTidalRooms_Text_JedIntro:
	.string "JED: I'm a little shy about this, but…\p"
	.string "We'll show you the power of our love!$"

SSTidalRooms_Text_JedDefeat:
	.string "JED: Sigh…$"

SSTidalRooms_Text_JedPostBattle:
	.string "JED: It's the first time our love power\n"
	.string "has ever failed!\p"
	.string "You must be an awesome TRAINER!$"

SSTidalRooms_Text_JedNotEnoughMons:
	.string "JED: You only have one POKéMON?\p"
	.string "Isn't that just too lonesome?$"

SSTidalRooms_Text_LeaIntro:
	.string "LEA: I feel a little silly, but…\p"
	.string "We'll show you the power of our love!$"

SSTidalRooms_Text_LeaDefeat:
	.string "LEA: Oh, boo!$"

SSTidalRooms_Text_LeaPostBattle:
	.string "LEA: I can't believe it!\p"
	.string "Our love power failed…\p"
	.string "You must be an awesome TRAINER!$"

SSTidalRooms_Text_LeaNotEnoughMons:
	.string "LEA: I wanted to battle…\p"
	.string "But you don't even have two POKéMON…$"

SSTidalRooms_Text_GarretIntro:
	.string "Ah, you've come just in time.\p"
	.string "I'm bored, you see. You may entertain\n"
	.string "me.$"

SSTidalRooms_Text_GarretDefeat:
	.string "…That will do.$"

SSTidalRooms_Text_GarretPostBattle:
	.string "Perhaps I shall ask Father to have\n"
	.string "CAPSULE CORP. build me a yacht.\p"
	.string "A yacht for me and my POKéMON!$"

SSTidalRooms_Text_NaomiIntro:
	.string "Oh, you're such an adorable TRAINER.\p"
	.string "Would you like to have tea? Or would you\n"
	.string "rather battle?$"

SSTidalRooms_Text_NaomiDefeat:
	.string "I see.\p"
	.string "You're the active sort.$"

SSTidalRooms_Text_NaomiPostBattle:
	.string "A world cruise on a luxury liner has its\n"
	.string "charms, I must say…\p"
	.string "But I will admit there's an appealing\n"
	.string "side to touring HOENN by ferry.$"

SSTidalRooms_Text_NotSuspiciousTakeThis:
	.string "U-uh, hi! I'm not acting suspicious!\n"
	.string "Really!\p"
	.string "H-here, take this! Free of charge!\p"
	.string "I swear I didn't SNATCH it off anybody!\n"
	.string "I'd never do such a thing! It's clean!\l"
	.string "You can use it!$"

SSTidalRooms_Text_ExplainSnatch:
	.string "SNATCH steals the beneficial effects\n"
	.string "of certain moves before a foe or ally\l"
	.string "can use them.$"

