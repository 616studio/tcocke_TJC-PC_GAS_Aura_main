// Copyright © 2026 616 Studio LLC. All Rights Reserved. ([https://616.studio](https://616.studio))


#include "Characters/X_CharacterClassInfo.h"

FX_CharacterClassDefaultInfo& UX_CharacterClassInfo::GetCharacterClassDefaultInfo(const ECharacterClass CharacterClass)
{
	return CharacterClassInformation.FindChecked(CharacterClass);
}