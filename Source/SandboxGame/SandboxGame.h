// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"

DECLARE_STATS_GROUP(TEXT("ECS Core Systems"), STATGROUP_SGCoreSystems, STATCAT_Advanced);

namespace SGMath
{
	/** Calculate the nth root of the given value */
	float SANDBOXGAME_API NthRoot(float value, float degree);
}
