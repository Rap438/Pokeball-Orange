SootopolisCity_House2_MapScripts::
	.byte 0

SootopolisCity_House2_EventScript_ExpertF::
	lock
	faceplayer
	msgbox SootopolisCity_House2_Text_DidYouKnowAboutMtPyreOrbs, MSGBOX_YESNO
	call_if_eq VAR_RESULT, YES, SootopolisCity_House2_EventScript_KnowAboutOrbs
	call_if_eq VAR_RESULT, NO, SootopolisCity_House2_EventScript_DontKnowAboutOrbs
	release
	end

SootopolisCity_House2_EventScript_KnowAboutOrbs::
	msgbox SootopolisCity_House2_Text_YesTwoOrbsSideBySide, MSGBOX_DEFAULT
	return

SootopolisCity_House2_EventScript_DontKnowAboutOrbs::
	msgbox SootopolisCity_House2_Text_OughtToVisitAndSee, MSGBOX_DEFAULT
	return

SootopolisCity_House2_Text_DidYouKnowAboutMtPyreOrbs:
	.string "MT. PYRE…\p"
	.string "At its peak, two orbs sit side by side.\n"
	.string "Did you know?$"

SootopolisCity_House2_Text_YesTwoOrbsSideBySide:
	.string "Yes, two orbs side by side…\p"
	.string "The sight of them together…\p"
	.string "It is somehow soothing…$"

SootopolisCity_House2_Text_OughtToVisitAndSee:
	.string "Is that so?\p"
	.string "Perhaps you ought to go and see for\n"
	.string "yourself…$"

