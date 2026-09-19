// Copyright © 2026 616 Studio LLC. All Rights Reserved. ([https://616.studio](https://616.studio))

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AttributeSet.h"
#include "GameplayEffectTypes.h"
#include "Engine/DataTable.h"
#include "X_CustomDataTypes.generated.h"

// Forward declarations
class UInputAction;
class UTexture2D;
class UAbilitySystemComponent;
class AActor;
class AController;
class ACharacter;
class UGameplayEffect;
class UBehaviorTree;

/**
 * <summary>
 * This file consolidates the declaration of all the different custom data types used across the project.
 * </summary>
 */

#pragma region Gameplay Effect Actor Specific

/**
 * <summary>
 * Determines <b>WHEN</b> a Gameplay Effect should be applied to a target.
 * </summary>
 * <remarks>
 * <b>CALLERS:</b>
 * <list type="bullet">
 * <item><description><c>X_GameplayEffectActor_Base</c></description></item>
 * </list>
 * </remarks>
 */
UENUM(BlueprintType)
enum class EGameplayEffectApplicationPolicy : uint8
{
    ApplyOnBeginOverlap,
    ApplyOnEndOverlap,
    DoNotApply
};

/**
 * <summary>
 * Determines <b>IF</b> an infinite Gameplay Effect should be removed.
 * </summary>
 * <remarks>
 * <b>CALLERS:</b>
 * <list type="bullet">
 * <item><description><c>X_GameplayEffectActor_Base</c></description></item>
 * </list>
 * </remarks>
 */
UENUM(BlueprintType)
enum class EGameplayEffectRemovalPolicy : uint8
{
    RemoveOnEndOverlap,
    DoNotRemove
};

#pragma endregion Gameplay Effect Actor Specific

#pragma region GAS Specific

/**
 * <summary>
 * Helper struct to extract data from the <c>FGameplayEffectModCallbackData</c> payload.
 * </summary>
 * <remarks>
 * <b>ARCHITECTURE NOTES:</b>
 * <list type="bullet">
 * <item><description>Caches the Instigator/Source (Attacker) and Target (Victim) actor data.</description></item>
 * <item><description>Uses raw pointers over <c>TObjectPtr</c> to prevent copy constructor bloat during high-frequency callbacks.</description></item>
 * </list>
 * <b>CALLERS:</b>
 * <list type="bullet">
 * <item><description><c>X_AttributeSet</c></description></item>
 * </list>
 * </remarks>
 */
USTRUCT()
struct FX_EffectProperties
{
    GENERATED_BODY()

    FX_EffectProperties(){}

    FGameplayEffectContextHandle EffectContextHandle;
	
    /* Target: target of the Gameplay Effect (The Victim). */
    /* Source: causer of the Gameplay Effect (The Attacker). */
    UPROPERTY()
    UAbilitySystemComponent* TargetASC = nullptr;

    UPROPERTY()
    UAbilitySystemComponent* SourceASC = nullptr;

    UPROPERTY()
    AActor* TargetAvatarActor = nullptr;

    UPROPERTY()
    AActor* SourceAvatarActor = nullptr;

    UPROPERTY()
    AController* TargetController = nullptr;

    UPROPERTY()
    AController* SourceController = nullptr;

    UPROPERTY()
    ACharacter* TargetCharacter = nullptr;

    UPROPERTY()
    ACharacter* SourceCharacter = nullptr;	
};

#pragma endregion GAS Specific

#pragma region Character Specific

/**
 * <summary>
 * Represents the different RPG classes that can be assigned to all Characters (Players and NPCs).
 * </summary>
 */
UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
    Elementalist,
    Warrior,
    Ranger,
    Hero,
    Unassigned
};

/**
 * <summary>
 * Helper struct to set the defaults associated with each <c>ECharacterClass</c>.
 * </summary>
 */
USTRUCT(BlueprintType)
struct FX_CharacterClassDefaultInfo
{
    GENERATED_BODY()
    
    /**
     * <summary>
     * The specific Behavior Tree to run when the specified Character Class enters Combat.
     * </summary>
     */
    UPROPERTY(EditDefaultsOnly, Category = "***CUSTOM|Class Defaults|AI")
    TObjectPtr<UBehaviorTree> CombatSubTree;

    /**
     * <summary>
     * Gameplay Effect used to set the initial values of the Primary Attributes: Intelligence, Strength, Resilience, and Vigor.
     * </summary>
     * <remarks>
     * <b>ARCHITECTURE NOTES:</b>
     * <list type="bullet">
     * <item><description><b>Modifier Op (Override):</b> We use Override because we want to force our initial Attributes to be a specific default value.</description></item>
     * <item><description><b>Modifier Magnitude (Scalable Float):</b> We use Scalable Float for now instead of a derived value from a curve table or custom calculation classes.</description></item>
     * </list>
     * </remarks>
     */
    UPROPERTY(EditDefaultsOnly, Category = "***CUSTOM|Class Defaults|Attributes")
    TSubclassOf<UGameplayEffect> PrimaryAttributes;
    
    /**
     * <summary>
     * Gameplay Effect used to update the values of the Secondary Attributes.
     * </summary>
     * <remarks>
     * <b>ARCHITECTURE NOTES:</b>
     * <list type="bullet">
     * <item><description><b>Modifier Op (Add (Base)) + Modifier Magnitude (Attribute Based):</b> Secondary Attributes derive their values from Primary Attributes.</description></item>
     * <item><description>Only MaxHealth and MaxMana are derived from custom calculation classes (<c>X_MMC_MaxHealth</c> and <c>X_MMC_MaxMana</c>).</description></item>
     * <item><description><b>Players (Hero Class):</b> The Duration Policy must be Infinite to maintain a "live link." This ensures the Secondary Attributes are updated dynamically whenever the Primary Attributes are changed.</description></item>
     * <item><description><b>NPCs:</b> Since NPCs do not gain levels or change their Primary Attributes during gameplay (for now), the Duration Policy is set to Instant.</description></item>
     
     * </list>
     * </remarks>
     */
    UPROPERTY(EditDefaultsOnly, Category = "***CUSTOM|Class Defaults|Attributes")
    TSubclassOf<UGameplayEffect> SecondaryAttributes;
    
    /**
     * <summary>
     * Gameplay Effect used to set the initial values of the Vital Attributes.
     * </summary>
     * <remarks>
     * <b>ARCHITECTURE NOTES:</b>
     * <list type="bullet">
     * <item><description><b>Modifier Op (Override) + Modifier Magnitude (Attribute Based):</b> The initial values of the Vital Attributes are derived from the Secondary Attributes MaxHealth and MaxMana.</description></item>
     * </list>
     * </remarks>
     */
    UPROPERTY(EditDefaultsOnly, Category = "***CUSTOM|Class Defaults|Attributes")
    TSubclassOf<UGameplayEffect> VitalAttributes;
    
    /**
     * <summary>
     * Gameplay Effect used to set the initial values of the Resistance Attributes.
     * </summary>
     * <remarks>
     * <b>ARCHITECTURE NOTES:</b>
     * <list type="bullet">
     * <item><description><b>Modifier Op (Override) + Modifier Magnitude (Attribute Base):</b> The initial values of the Resistance Attributes are derived from the Primary Attributes.</description></item>
     * <item><description><b>Players (Hero Class):</b> The Duration Policy must be Infinite to maintain a "live link." This ensures the Secondary Attributes are updated dynamically whenever the Primary Attributes are changed.</description></item>
     * <item><description><b>NPCs:</b> Since NPCs do not gain levels or change their Primary Attributes during gameplay (for now), the Duration Policy is set to Instant.</description></item>
     * </list>
     * </remarks>
     */
    UPROPERTY(EditDefaultsOnly, Category = "***CUSTOM|Class Defaults|Attributes")
    TSubclassOf<UGameplayEffect> ResistanceAttributes;
    
    /**
     * <summary>
     * Infinite Gameplay Effect used to assign default Gameplay Tags to each <c>ECharacterClass</c>.
     * </summary>
     */
    UPROPERTY(EditDefaultsOnly, Category = "***CUSTOM|Class Defaults")
    TSubclassOf<UGameplayEffect> DefaultGameplayTags;

    /**
     * <summary>
     * Default Gameplay Abilities assigned to each <c>ECharacterClass</c>.
     * </summary>
     */
    UPROPERTY(EditDefaultsOnly, Category = "***CUSTOM|Class Defaults|Abilities")
    TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;

    /**
     * <summary>
     * Allows a class to start with no default Gameplay Abilities.
     * </summary>
     */
    UPROPERTY(EditDefaultsOnly, Category = "***CUSTOM|Class Defaults|Abilities")
    bool bShouldHaveDefaultAbilities = false;
};

/**
 * <summary>
 * Helper struct to map Gameplay Tags to AnimMontages.
 * </summary>
 */
USTRUCT(BlueprintType)
struct FX_GameplayTaggedAnimMontage
{
    GENERATED_BODY()

    /**
     * <summary>
     * The AnimMontage specific to the attack behavior we want the Character to play.
     * </summary>
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TObjectPtr<UAnimMontage> AnimMontage;

    /**
     * <summary>
     * The Gameplay Tag to identify the socket associated with the AnimMontage that will be used to apply damage.
     * </summary>
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FGameplayTag SocketLocationTag;

    /**
     * <summary>
     * The Gameplay Tag to identify the type of attack being performed (melee, projectile, spell).
     * </summary>
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FGameplayTag AttackType;
    
    /**
     * <summary>
     * Time in seconds from the start of the Montage until the attack physically hits.
     * </summary>
     * <remarks>
     * <b>ARCHITECTURE NOTES:</b>
     * <list type="bullet">
     * <item><description><b>Server Optimization:</b> Replaces AnimNotifies for server-side combat calculation, allowing the server to disable skeletal mesh ticking entirely for off-screen enemies to save massive CPU overhead.</description></item>
     * <item><description>Used by <c>UAbilityTask_WaitDelay</c> in the Gameplay Ability to trigger the damage payload.</description></item>
     * </list>
     * </remarks>
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float ImpactDelay = 0.5f;
};

#pragma endregion Character Specific

#pragma region UI

/**
 * <summary>
 * Defines the row structure used by the Data Table asset <c>DT_ScreenMessage_ItemPickup</c> .
 * </summary>
 * <remarks>
 * <b>ARCHITECTURE NOTES:</b>
 * <list type="bullet">
 * <item><description>Each row contains the text to be displayed (<c>Message</c>), the image to be displayed (<c>Image</c>), and the View class to be used (<c>WBP_View_ScreenMessage_ItemPickup</c>) to display the data.</description></item>
 * </list>
 * <b>IMPORTANT:</b>
 * <list type="bullet">
 * <item><description>The <c>Row Name</c> in the Data Table <b>MUST</b> match the <c>MessageTag</c> exactly for the <c>FindRow</c> lookup logic to succeed.</description></item>
 * <item><description><c>MessageTag</c> must match the Asset Tag of the Gameplay Effect tied to the item being picked up.</description></item>
 * </list>
 * </remarks>
 */
USTRUCT(BlueprintType)
struct FX_ScreenMessageItemPickupRowStructure : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag MessageTag = FGameplayTag();
    
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Message = FText();
    
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> Image;
    
	// Forward declare the class here so we don't need to include the massive View base class header.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftClassPtr<class UX_UI_View_Base> View; 
};

/**
 * <summary>
 * Maps an <c>FGameplayAttribute</c> to an <c>FGameplayTag</c> and its localized display information.
 * </summary>
 */
USTRUCT(BlueprintType)
struct FX_AttributeDisplayInfoContainer
{
	GENERATED_BODY()
	
	/**
	 * <summary>
	 * The unique <c>FGameplayTag</c> associated with this specific Gameplay Attribute.
	 * </summary>
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AttributeTag = FGameplayTag();
	
	/**
	 * <summary>
	 * The native GAS Gameplay Attribute.
	 * </summary>
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayAttribute Attribute;

	/**
	 * <summary>
	 * The localized name of the Gameplay Attribute.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>Auto-populated in <c>X_AttributeDisplayInfo</c> from the Gameplay Tag specified in <c>X_GameplayTags</c>.</description></item>
	 * </list>
	 * </remarks>
	 */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	FText AttributeName = FText();

	/**
	 * <summary>
	 * The localized description explaining what the Gameplay Attribute does.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>Auto-populated in <c>X_AttributeDisplayInfo</c> from the comments specified in <c>X_GameplayTags</c>.</description></item>
	 * </list>
	 * </remarks>
	 */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	FText AttributeDescription = FText();

	/**
	 * <summary>
	 * The current numeric value of the Gameplay Attribute, populated dynamically at runtime.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description><b>Runtime Only:</b>  This value is not configured in the Data Asset, but is injected by the Controller right before it is broadcast.</description></item>
	 * </list>
	 * </remarks>
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float AttributeValue = 0.0f;
	
};

/**
 * <summary>
 * Used to consolidate broadcasts of CurrentValue and MaxValue Attributes into a single payload for Progress Bar specific UI elements to avoid "frame 0" initial View state synchronization issues.
 * </summary>
 * <remarks>
 * <b>ARCHITECTURE NOTES:</b>
 * <list type="bullet">
 * <item><description>Prevents MaxValue arriving before CurrentValue to avoid SafeDivide calls that result in a percentage value of 0, forcing Progress Bar specific UI elements to appear empty on "frame 0" of UI initialization.</description></item>
 * </list>
 * </remarks>
 */
USTRUCT(BlueprintType)
struct FX_AttributeDisplayProgressBarContainer
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "UI|Attributes")
	float CurrentValue = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "UI|Attributes")
	float MaxValue = 0.0f;
};

/**
 * <summary>
 * Struct payload used to transport the Models of the MVC UI architecture (<c>PC</c>, <c>PS</c>, <c>ASC</c>, and <c>AS</c>) to Controllers.
 * </summary>
 * <remarks>
 * <b>ARCHITECTURE NOTES:</b>
 * <list type="bullet">
 * <item><description>Uses base Engine pointers rather than derived classes to avoid hardcoded dependencies.</description></item>
 * <item><description>Any required casting to derived classes is handled internally by the receiving Controller.</description></item>
 * </list>
 * </remarks>
 */
USTRUCT(BlueprintType)
struct FX_ModelsPayload
{
	GENERATED_BODY()

	FX_ModelsPayload() {}
	FX_ModelsPayload(
		APlayerController* InPlayerController,
		APlayerState* InPlayerState,
		UAbilitySystemComponent* InAbilitySystemComponent,
		UAttributeSet* InAttributeSet
		)
		:
		PlayerController(InPlayerController),
		PlayerState(InPlayerState),
		AbilitySystemComponent(InAbilitySystemComponent),
		AttributeSet(InAttributeSet)
	{}

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<APlayerState> PlayerState;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAttributeSet> AttributeSet;
};

#pragma endregion UI

#pragma region Input

/**
 * <summary>
 * Maps Input Actions to Gameplay Tags.
 * </summary>
 */
USTRUCT(BlueprintType)
struct FX_InputActionGameplayTag
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, Category="***CUSTOM|Input")
	TObjectPtr<UInputAction> InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly, Category="***CUSTOM|Input")
	FGameplayTag InputTag = FGameplayTag();
};

#pragma endregion Input