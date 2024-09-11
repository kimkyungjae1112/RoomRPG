// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_SkillHitCheck.h"
#include "Interface/RAnimationAttackInterface.h"

void UAnimNotify_SkillHitCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		IRAnimationAttackInterface* SkillPawn = Cast<IRAnimationAttackInterface>(MeshComp->GetOwner());
		if (SkillPawn)
		{
			UE_LOG(LogTemp, Display, TEXT("스킬 실행"));
			SkillPawn->R_SkillHitCheck();
		}
	}
}
