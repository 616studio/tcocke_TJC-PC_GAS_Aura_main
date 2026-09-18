// Copyright © 2026 616 Studio LLC. All Rights Reserved. (https://616.studio)

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GAS_Aura/UtilityClasses/X_CustomDataTypes.h"
#include "GameFramework/Character.h"
#include "Interfaces/X_CharacterInterface.h"
#include "Interfaces/X_CursorHighlightInterface.h"
#include "X_Character_Base.generated.h"

class UAttributeSet;

/**
 * <summary>
 * Abstract Base Class for all Characters (Players and NPCs).
 * </summary>
 */
UCLASS(Abstract)
class GAS_AURA_API AX_Character_Base : public ACharacter, public IAbilitySystemInterface, public IX_CursorHighlightInterface, public IX_CharacterInterface
{
	GENERATED_BODY()
	
#pragma region Unreal Defaults

public:
	AX_Character_Base();
	
	virtual void BeginPlay() override;

protected:
	
private:

#pragma endregion Unreal Defaults
	
#pragma region Ability System
	
public:
	
	/**
	 * <summary>
	 * <c>IAbilitySystemInterface</c> implementation. Returns the Ability System Component associated with this Character.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description><b>Players:</b> Derived class <c>X_Character_Player</c> returns the ASC declared and constructed in <c>X_PlayerState</c>.</description></item>
	 * <item><description><b>NPCs:</b> Derived class <c>X_Character_NPC</c> returns the ASC declared and constructed in <c>X_Character_NPC</c>.</description></item>
	 * </list>
	 * </remarks>
	 */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	/**
	 * <summary>
	 * Returns the Attribute Set associated with this Character.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>This is our own specific getter just for the AS, not part of any interface.</description></item>
	 * <item><description><b>Players:</b> Derived class <c>X_Character_Player</c> overrides this function and returns the AS declared and constructed in <c>X_PlayerState</c>.</description></item>
	 * <item><description><b>NPCs:</b> Derived class <c>X_Character_NPC</c> overrides this function and returns the AS declared and constructed in <c>X_Character_NPC</c>.</description></item>
	 * </list>
	 * </remarks>
	 */
	virtual UAttributeSet* GetAttributeSet() const;
	
	/**
	* <summary>
	* <c>IX_CharacterInterface</c> implementation.  Returns the Character's Level for derived classes.
	* </summary>
	* <remarks>
	* <b>ARCHITECTURE NOTES:</b>
	* <list type="bullet">
	* <item><description><b>Players:</b> Derived class <c>X_Character_Player</c> returns the Character's Level declared and managed in <c>X_PlayerState</c>.</description></item>
	* <item><description><b>NPCs:</b> Derived class <c>X_Character_NPC</c> returns the Character's Level declared and managed in <c>X_Character_NPC</c>.</description></item>
	* </list>
	* </remarks>
	*/
	virtual int32 GetCharacterLevel() const override { return 1; };

	/**
	 * <summary>
	 * <c>IX_CharacterInterface</c> implementation.  Sets the Character's Level for derived classes.
	 * </summary>	 
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description><b>Players:</b> Derived class <c>X_Character_Player</c> sets the Character's Level declared and managed in <c>X_PlayerState</c>.</description></item>
	 * <item><description><b>NPCs:</b> Derived class <c>X_Character_NPC</c> sets the Character's Level declared and managed in <c>X_Character_NPC</c>.</description></item>
	 * </list>
	 * </remarks>
	 */
	virtual void SetCharacterLevel(const int32 NewLevel) override;
	
	/**
	 * <summary>
	 * <c>IX_CharacterInterface</c> implementation.  Returns the Character's Class for derived classes.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description><b>Players:</b> Derived class <c>X_Character_Player</c> returns the Character's Class declared and managed in <c>X_PlayerState</c>.</description></item>
	 * <item><description><b>NPCs:</b> Derived class <c>X_Character_NPC</c> returns the Character's Class declared and managed in <c>X_Character_NPC</c>.</description></item>
	 * </list>
	 * </remarks>
	 */
	virtual ECharacterClass GetCharacterClass() const override { return ECharacterClass::Unassigned; }
	
	/**
	 * <summary>
	 * <c>IX_CharacterInterface</c> implementation.  Sets the Character's Class for derived classes.
	 * </summary>	 
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description><b>Players:</b> Derived class <c>X_Character_Player</c> sets the Character's Class declared and managed in <c>X_PlayerState</c>.</description></item>
	 * <item><description><b>NPCs:</b> Derived class <c>X_Character_NPC</c> sets the Character's Class declared and managed in <c>X_Character_NPC</c>.</description></item>
	 * </list>
	 * </remarks>
	 */
	virtual void SetCharacterClass(const ECharacterClass NewClass) override;
	
protected:
	
	/**
	 * <summary>
	 * Allows the <c>X_Character_Player</c> and <c>X_Character_NPC</c> to determine in their own classes when and where to initialize all their Attributes.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description><b>Server-Side Only:</b> Called from <c>PossessedBy</c> in both <c>X_Character_Player</c> and <c>X_Character_NPC</c>.</description></item>
	 * <item><description>Utilizes Data Asset <c>DA_CharacterClassInfo</c> stored on the Game State (<c>BP_GameState_Base</c>).</description></item>
	 * </list>
	 * </remarks>
	 * <param name="InSourceObject">[<c>UObject*</c>]: The Object that owns the effect.</param>
	 * <param name="InInstigator">[<c>AActor*</c>]: The physical Actor that instigated the effect.</param>
	 * <param name="InEffectCauser">[<c>AActor*</c>]: The physical Actor that caused the effect.</param>
	 * <param name="InCharacterClass">[<c>ECharacterClass</c>]: The Character's current Class.</param>
	 * <param name="InCharacterLevel">[<c>int32</c>]: The Character's current Level.</param>
	 */
	virtual void InitializeAttributes(UObject* InSourceObject, AActor* InInstigator, AActor* InEffectCauser, const ECharacterClass InCharacterClass, const int32 InCharacterLevel);
	
	/**
	 * <summary>
	 * Grants the assigned default Gameplay Abilities to the Character based on their Character's Class.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description><b>Server-Side Only:</b> Called from <c>PossessedBy</c> in both <c>X_Character_Player</c> and <c>X_Character_NPC</c>.</description></item>
	 * <item><description>Utilizes Data Asset <c>DA_CharacterClassInfo</c> stored on the Game State (<c>BP_GameState_Base</c>).</description></item>
	 * </list>
	 * </remarks>
	 * <param name="InCharacterLevel">[<c>cont int32</c>]: The Level to grant the Gameplay Abilities at based on the Character's current Level.</param>
	 * <param name="InCharacterClass">[<c>const ECharacterClass</c>]: The Character's current Class used to query <c>DA_CharacterClassInfo</c>.</param>
	 */
	void GrantClassDefaultGameplayAbilitiesOnStartup(const ECharacterClass InCharacterClass, const int32 InCharacterLevel);
	
	/**
	 * <summary>
	 * Grants the assigned shared Gameplay Abilities to the Character common to each <c>ECharacterClass</c>.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description><b>Server-Side Only:</b> Called from <c>PossessedBy</c> in both <c>X_Character_Player</c> and <c>X_Character_NPC</c>.</description></item>
	 * <item><description>Utilizes Data Asset <c>DA_CharacterClassInfo</c> stored on the Game State (<c>BP_GameState_Base</c>).</description></item>
	 * </list>
	 * </remarks>
	 * <param name="InCharacterLevel">[<c>int32</c>]: The Level to grant the abilities at based on the Character's current Level.</param>
	 */
	void GrantClassSharedGameplayAbilitiesOnStartup(int32 InCharacterLevel);

	/**
	 * <summary>
	 * Grants default Gameplay Tags to the Character based on the Character's Class.
	 * </summary>
	 * <remarks>
	 * </remarks>
	 * <param name="InSourceObject">[<c>UObject*</c>]: The Object that owns the effect.</param>
	 * <param name="InInstigator">[<c>AActor*</c>]: The physical Actor that instigated the effect.</param>
	 * <param name="InEffectCauser">[<c>AActor*</c>]: The physical Actor that caused the effect.</param>
	 * <param name="InCharacterClass">[<c>ECharacterClass</c>]: The Character's current Class.</param>
	 * <param name="InCharacterLevel">[<c>const int32</c>]: The Level used to scale the Attributes based on the Character's current Level.</param> 
	 */
	virtual void InitializeDefaultGameplayTags(UObject* InSourceObject, AActor* InInstigator, AActor* InEffectCauser, ECharacterClass InCharacterClass, const int32 InCharacterLevel);
	
	/**
	 * <summary>
	 * Helper function available to all derived classes used to apply Gameplay Effects to the ASC of the Character itself.
	 * </summary>
	 * <param name="GameplayEffect">[<c>TSubclassOf</c>(<c>UGameplayEffect</c>)]: The specific Gameplay Effect class to apply.</param>
	 * <param name="SourceObject">[<c>UObject*</c>]: The Object applying the effect.</param>
	 * <param name="InInstigator">[<c>AActor*</c>]: The physical Actor that instigated the effect.</param>
	 * <param name="InEffectCauser">[<c>AActor*</c>]: The physical Actor that caused the effect.</param>
	 * <param name="InCharacterLevel">[<c>int32</c>]: The Level of the Character used to scale the effect.</param>
	 */
	void ApplyGameplayEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffect, const UObject* SourceObject, AActor* InInstigator, AActor* InEffectCauser, int32 InCharacterLevel) const;
	
private:
	
#pragma endregion Ability System
	
#pragma region Character Defaults
	
public:
		
protected:

	/**
	 * <summary>
	 * Used to apply/prevent pushback by certain Characters.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description><b>Players:</b> <c>1000.0f</c></description></item>
	 * <item><description><b>NPCs (Regular): </b> <c>100.0f</c></description></item>
	 * <item><description><b>NPCs (Bosses): </b> <c>10000.0f</c></description></item>
	 * </list>
	 * <b>IMPORTANT:</b>
	 * <list type="bullet">
	 * <item><description>This only works when applying math/logic to prevent Characters from forcibly walking into each other by allowing AI to dynamically calculate their stopping distance based on their physical size (see <c>ChaseAcceptanceRadius</c>).</description></item>
	 * </list>
	 * </remarks>
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "***CUSTOM|Combat")
	float CharacterMass = 100.0f;
	
private:
	
	/**
	 * <summary>
	 * Helper function to initialize default values specific to the Character's desired default behavior as declared in this pragma region.
	 * </summary>
	 */
	void InitCharacterBehavior();
	
#pragma endregion Character Defaults
	
#pragma region Weapon
	
public:
	
protected:
	
	/**
	 * <summary>
	 * The skeletal mesh used for the Character's weapon.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>Uses a Skeletal Mesh Component instead of a Static Mesh Component because some of the weapons include bones (Slingshots, for example).</description></item>
	 * </list>
	 * </remarks>
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "***CUSTOM|Weapon")
	TObjectPtr<USkeletalMeshComponent> Weapon;
	
	/**
	 * <summary>
	 * Socket attached to the Skeletal Mesh Component of the Character.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>Can also be used for the spawn location of a projectile.</description></item>
	 * <item><description>For all the models in this project, their animations are designed for weapons to be attached to their left hand.</description></item>
	 * </list>
	 * </remarks>
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "***CUSTOM|Weapon")
	FName WeaponHandSocket = "WeaponHandSocket";
	
	/**
	 * <summary>
	 * Socket used for tracing/spawning a projectile from a location on the Skeletal Mesh Asset assigned to a Weapon's mesh.
	 * </summary>
	 */
	UPROPERTY(EditAnywhere, Category = "***CUSTOM|Weapon|Sockets")
	FName WeaponProjectileSocketName = "WeaponProjectileSocket";
	
private:
	
	/**
	 * <summary>
	 * Consolidated helper function for initializing the Character's weapon.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>This function contains <c>CreateDefaultSubobject</c> and <b>MUST ONLY</b> be called from the Constructor!</description></item>
	 * </list>
	 * </remarks>
	 */
	void InitWeapon();
	
#pragma endregion Weapon
	
#pragma region Cursor Highlight Behavior
	
public:
	
	/**
	 * <summary>
	 * <c>IX_CursorHighlightInterface</c> required implementation.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>When the mouse cursor hovers over an actor that implements the interface, an outline is drawn around them.</description></item>
	 * </list>
	 * </remarks>
	 */
	virtual void AddHighlightToActor() override;
	
	/**
	 * <summary>
	 * <c>IX_CursorHighlightInterface</c> required implementation.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>Removes the outline originally applied by <c>AddHighlightToActor</c> when the mouse is no longer hovering over an actor that implements the interface.</description></item>
	 * </list>
	 * </remarks>
	 */
	virtual void RemoveHighlightFromActor() override;
	
	/**
	 * <summary>
	 * <c>IX_CursorHighlightInterface</c> required implementation.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>Returns default value of <c>250.0f</c> for the color <c>Red</c> in order to work with the custom PP_Highlight material.</description></item>
	 * <item><description><b>IMPORTANT:</b> Will <b>NOT</b> work unless you enable it in Project Settings - Engine - Rendering - Post Processing:  Custom Depth - Stencil Pass -> Enabled with Stencil</description></item>
	 * </list>
	 * </remarks>
	 */
	virtual float GetCustomStencilDepthValue() const override;
	
protected:
	
private:
	
#pragma endregion Cursor Highlight Behavior
	
};