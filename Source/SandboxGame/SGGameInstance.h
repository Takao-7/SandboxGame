// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ECSGameInstance.h"
#include "SGGameInstance.generated.h"


/**
 * 
 */
UCLASS()
class SANDBOXGAME_API USGGameInstance : public UECSGameInstance
{
	GENERATED_BODY()

public:
    virtual void Init() override;
};
