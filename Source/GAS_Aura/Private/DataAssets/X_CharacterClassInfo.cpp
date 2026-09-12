// Copyright © 2026 616 Studio LLC. All Rights Reserved. ([https://616.studio](https://616.studio))

#include "DataAssets/X_CharacterClassInfo.h"

const FX_CharacterClassDefaultInfo* UX_CharacterClassInfo::GetCharacterClassDefaultInfo(const ECharacterClass CharacterClass) const
{
	return CharacterClassInformation.Find(CharacterClass);
}