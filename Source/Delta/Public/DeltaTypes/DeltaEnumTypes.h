// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//Template Code: Copy and edit following lines
/*
UENUM(BlueprintType)
enum class EDeltaTemplateType : uint8
{
	Type1,
	Type2,
	
	Max UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(EDeltaTemplateType, EDeltaTemplateType::Max);
*/

UENUM(BlueprintType)
enum class EDeltaSkillType : uint8
{
	//Playable Character
	Parrying,
	
	//Back Character
	Shield,
	MeteorShower,

	//Desert Dragon
	LeftClaws,
	RightClaws,
	DoubleClaws,
	Bite,
	SpitFire,
	SpreadFire,
	
	Max UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(EDeltaSkillType, EDeltaSkillType::Max);


UENUM(BlueprintType)
enum class EBlackboardKey : uint8
{
	TargetActor
};

inline FName GetBlackboardKeyName(const EBlackboardKey KeyName)
{
	switch (KeyName)
	{
	case EBlackboardKey::TargetActor: return FName("TargetActor");
	default: return NAME_None;
		
	}
}
