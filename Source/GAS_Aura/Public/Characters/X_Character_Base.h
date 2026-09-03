// Copyright © 2026 616 Studio LLC. All Rights Reserved. (https://616.studio)

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/X_GAS_DataTypes.h"
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
	* <c>IX_CharacterInterface</c> implementation.  Returns the CharacterLevel for derived classes.
	* </summary>
	* <remarks>
	* <b>ARCHITECTURE NOTES:</b>
	* <list type="bullet">
	* <item><description><b>Players:</b> Derived class <c>X_Character_Player</c> returns the CharacterLevel declared and managed in <c>X_PlayerState</c>.</description></item>
	* <item><description><b>NPCs:</b> Derived class <c>X_Character_NPC</c> returns the CharacterLevel declared and managed in <c>X_Character_NPC</c>.</description></item>
	* </list>
	* </remarks>
	*/
	virtual int32 GetCharacterLevel() const override;
	
	/**
	 * <summary>
	 * Returns the Character's class.
	 * </summary>
	 */
	virtual ECharacterClassType GetCharacterClass() { return CharacterClass; }
	
protected:
	
	/**
	 * <summary>
	 * The Character's class.
	 * </summary>
	 */
	UPROPERTY(EditDefaultsOnly, Category = "***CUSTOM|GAS|Character Class")
	ECharacterClassType CharacterClass = ECharacterClassType::Warrior;
	
	/**
	 * <summary>
	 * Allows the <c>X_Character_Player</c> and <c>X_Character_NPC</c> to determine in their own classes when and where to initialize all their Attributes.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>Called from <c>PossessedBy</c> in both <c>X_Character_Player</c> and <c>X_Character_NPC</c>.</description></item>
	 * <item><description>Utilizes Data Asset <c>DA_CharacterClassInfo</c> stored on the Game Mode (<c>BP_GameMode</c>).</description></item>
	 * </list>
	 * </remarks>
	 * <param name="SourceObject">[<c>const UObject*</c>]: The object that owns the effect.</param>
	 * <param name="InInstigator">[<c>AActor*</c>]: The actor that instigated the effect.</param>
	 * <param name="InEffectCauser">[<c>AActor*</c>]: The physical actor that caused the effect.</param>
	 * <param name="InCharacterLevel">[<c>int32</c>]: The level used to scale the Attributes.</param>
	 */
	virtual void InitializeAttributes(UObject* SourceObject, AActor* InInstigator, AActor* InEffectCauser, int32 InCharacterLevel);
	
		/**
	 * <summary>
	 * Grants the assigned default Gameplay Abilities to the Character based on their <c>CharacterClass</c>.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>Called from <c>PossessedBy</c> in both <c>X_Character_Player</c> and <c>X_Character_NPC</c>.</description></item>
	 * <item><description>Utilizes Data Asset <c>DA_CharacterClassInfo</c> stored on the Game Mode.</description></item>
	 * </list>
	 * </remarks>
	 * <param name="InCharacterLevel">[<c>int32</c>]: The level to grant the abilities at.</param>
	 */
	void GrantClassDefaultGameplayAbilitiesOnStartup(int32 InCharacterLevel);
	
	/**
	 * <summary>
	 * Grants the assigned shared Gameplay Abilities to the Character common to each <c>ECharacterClassType</c>.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>Called from <c>PossessedBy</c> in both <c>X_Character_Player</c> and <c>X_Character_NPC</c>.</description></item>
	 * <item><description>Utilizes Data Asset <c>DA_CharacterClassInfo</c> stored on the Game Mode.</description></item>
	 * </list>
	 * </remarks>
	 * <param name="InCharacterLevel">[<c>int32</c>]: The level to grant the abilities at.</param>
	 */
	void GrantClassSharedGameplayAbilitiesOnStartup(int32 InCharacterLevel);

	/**
	 * <summary>
	 * Grants default Gameplay Tags to the Character based on their <c>CharacterClass</c>.
	 * </summary>
	 * <remarks>
	 * </remarks>
	 * <param name="SourceObject">[<c>const UObject*</c>]: The object that owns the effect.</param>
	 * <param name="InInstigator">[<c>AActor*</c>]: The actor that instigated the effect.</param>
	 * <param name="InEffectCauser">[<c>AActor*</c>]: The physical actor that caused the effect.</param>
	 * <param name="InCharacterLevel">[<c>int32</c>]: The level used to scale the Attributes.</param> 
	 */
	virtual void InitializeDefaultGameplayTags(UObject* SourceObject, AActor* InInstigator, AActor* InEffectCauser, int32 InCharacterLevel);
	
	/**
	 * <summary>
	 * Helper function to initialize all Attributes for a Character by applying them as Gameplay Effects to the ASC of the Character itself.
	 * </summary>
	 * <param name="ASC">[<c>UAbilitySystemComponent*</c>]: The Target ASC.</param>
	 * <param name="GameplayEffect">[<c>TSubclassOf</c>(<c>UGameplayEffect</c>)]: The specific Gameplay Effect class to apply.</param>
	 * <param name="SourceObject">[<c>const UObject*</c>]: The Object applying the effect.</param>
	 * <param name="InInstigator">[<c>AActor*</c>]: The actor that instigated the effect.</param>
	 * <param name="InEffectCauser">[<c>AActor*</c>]: The physical actor that caused the effect.</param>
	 * <param name="InCharacterLevel">[<c>int32</c>]: The level of the Character used to scale the effect.</param>
	 */
	void ApplyGameplayEffectToSelf(UAbilitySystemComponent* ASC, TSubclassOf<UGameplayEffect> GameplayEffect, const UObject* SourceObject, AActor* InInstigator, AActor* InEffectCauser, int32 InCharacterLevel) const;
	
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
	 */
	void InitWeapon();
	
#pragma endregion Weapon
	
#pragma region UI
	
public:
	
	/**
	 * <summary>
	 * Called by the Server to broadcast a damage event to ALL clients so they can render their floating damage text user widget locally.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>Uses NetMulticast so the server knows to broadcast to all clients.</description></item>
	 * <item><description>Uses Unreliable network routing to save bandwidth during high-volume AoE combat.</description></item>
	 * </list>
	 * </remarks>
	 */
	UFUNCTION(NetMulticast, Unreliable)
	virtual void MulticastRPC_BroadcastDamage(const float DamageAmount, const FVector TargetLocation, AActor* InstigatorActor, const FGameplayTag MessageTag, const FGameplayTag AbilityTag);
	
protected:
	
private:
	
#pragma endregion UI

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