// Copyright © 2026 616 Studio LLC. All Rights Reserved. ([https://616.studio](https://616.studio))

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/X_GAS_DataTypes.h"
#include "UObject/Interface.h"
#include "X_CharacterInterface.generated.h"


// This class does not need to be modified.
UINTERFACE()
class UX_CharacterInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GAS_AURA_API IX_CharacterInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	/**
	 * <summary>
	 * Retrieves the Character's current Level.
	 * </summary>
	 */
	virtual int32 GetCharacterLevel() const = 0;

	/**
	 * <summary>
	 * Sets the Character's Level.
	 * </summary> 
	 */
	virtual void SetCharacterLevel(const int32 NewLevel) = 0;

	/**
	 * <summary>
	 * Retrieves the Character's current Class.
	 * </summary> 
	 */
	virtual ECharacterClass GetCharacterClass() const = 0;

	/**
	 * <summary>
	 * Sets the Character's Class.
	 * </summary>	 
	 */
	virtual void SetCharacterClass(const ECharacterClass NewClassType) = 0;
};
