// Copyright © 2026 616 Studio LLC. All Rights Reserved. ([https://616.studio](https://616.studio))

#pragma once

#include "CoreMinimal.h"

/**
 * <summary>
 * Centralized location for all custom Collision Channels and Trace Profiles.
 * </summary>
 * <remarks>
 * <b>ARCHITECTURE NOTES:</b>
 * <list type="bullet">
 * <item><description><b>Trace Channels:</b> Mapped directly from <c>Config/DefaultEngine.ini</c> under <c>[/Script/Engine.CollisionProfile]</c>.</description></item>
 * </list>
 * </remarks>
 */

// Object Channels
#define ECC_PlayerCapsule ECC_GameTraceChannel1
#define ECC_EnemyCapsule ECC_GameTraceChannel2
#define ECC_PlayerTrigger ECC_GameTraceChannel3
#define ECC_NavigableMesh ECC_GameTraceChannel4
#define ECC_Projectile ECC_GameTraceChannel5
#define ECC_GlobalTrigger ECC_GameTraceChannel6

// Trace Channels
#define ECC_Highlightable ECC_GameTraceChannel7
#define ECC_NavMeshPathCheck ECC_GameTraceChannel8
