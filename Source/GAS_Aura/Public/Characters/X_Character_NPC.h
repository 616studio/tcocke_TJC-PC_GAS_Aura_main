// Copyright © 2026 616 Studio LLC. All Rights Reserved. ([https://616.studio](https://616.studio))

#pragma once

#include "CoreMinimal.h"
#include "X_Character_Base.h"
#include "X_Character_NPC.generated.h"

UCLASS()
class GAS_AURA_API AX_Character_NPC : public AX_Character_Base
{
	GENERATED_BODY()

#pragma region Unreal Defaults
	
public:
	AX_Character_NPC();
	
	virtual void BeginPlay() override;

protected:

private:
	
#pragma endregion Unreal Defaults
	
#pragma region Ability System
	
public:
	
	/**
	 * <summary>
	 * <c>IAbilitySystemInterface</c> implementation override. Returns the Ability System Component associated with this Character.
	 * </summary>
	 */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	/**
	 * <summary>
	 * Returns the Attribute Set associated with this Character.
	 * </summary>
	 */
	virtual UAttributeSet* GetAttributeSet() const override;
	
	/**
	 * <summary>
	 * Used for the client-side initialization logic required for the Ability System.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>Runs after all subobjects (like the ASC and AttributeSet) are fully created and serialized, but before <c>BeginPlay</c> is called.</description></item>
	 * <item><description>Ensures the local Player sees the correct NPC stats as soon as it spawns.</description></item>
	 * </list>
	 * </remarks>
	 */
	virtual void PostInitializeComponents() override;

	/**
	 * <summary>
	 * Used for the server-side initialization logic required for the Ability System.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>Called on the Server when the AI Controller officially takes over the Pawn.</description></item>
	 * <item><description>This guarantees the Controller is valid before we initialize the Ability System.</description></item>
	 * </list>
	 * </remarks>
	 * <param name="NewController">[<c>AController*</c>]: The AI Controller possessing this NPC.</param>
	 */
	virtual void PossessedBy(AController* NewController) override;
	
protected:
	
	/**
	 * <summary>
	 * The base pointer for the Ability System Component.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>Declared as a base pointer strictly for header decoupling (prevent unnecessary includes / header contagion).</description></item>
	 * <item><description>Instantiates this as <c>UX_AbilitySystemComponent</c> to utilize our subclassed functionality (such as binding to custom Gameplay Effect delegates).</description></item>
	 * </list>
	 * </remarks>
	 */
	UPROPERTY(VisibleAnywhere, Category = "***CUSTOM|GAS|Components")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	/**
	 * <summary>
	 * The base pointer for the Attribute Set.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>Declared as a base pointer strictly for header decoupling (prevent unnecessary includes / header contagion).</description></item>
	 * <item><description>Instantiates this as <c>UX_AttributeSet</c> to access our subclassed <c>FGameplayAttributeData</c> properties and their network replication functions.</description></item>
	 * </list>
	 * </remarks>
	 */
	UPROPERTY(VisibleAnywhere, Category = "***CUSTOM|GAS|Components")
	TObjectPtr<UAttributeSet> AttributeSet;
	
private:
	
	/**
	 * <summary>
	 * Helper function to consolidate client-side and server-side initialization for the ASC and AS.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>Called inside <c>PostInitializeComponents</c> (for the Client) and <c>PossessedBy</c> (for the Server).</description></item>
	 * </list>
	 * </remarks>
	 */
	void InitAbilitySystemServerAndClientSide();
	
	/**
	 * <summary>
	 * Race condition guard for Attributes and Startup Gameplay Abilities.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>Tracks whether Attributes and Startup Gameplay Abilities have already been initialized.</description></item>
	 * <item><description>Prevents duplicate Gameplay Effects and Gameplay Ability specs from being created if the NPC is ever re-possessed (ex: swapping AI Controllers at runtime).</description></item>
	 * </list>
	 * </remarks>
	 */
	bool bAbilitySystemInitialized = false;
	
	/**
	 * <summary>
	 * Helper function to create and initialize components specific to the Ability System.
	 * </summary>
	 * <remarks>
	 * <b>ARCHITECTURE NOTES:</b>
	 * <list type="bullet">
	 * <item><description>This function contains <c>CreateDefaultSubobject</c> and <b>MUST ONLY</b> be called from the Constructor!</description></item>
	 * </list>
	 * </remarks>
	 */
	void InitComponentsForAbilitySystem();
	
#pragma endregion Ability System
	
};
