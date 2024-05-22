// Fill out your copyright notice in the Description page of Project Settings.

#include "QuestManager.h"
#include "QuestSubject.h"

void FQuestObject::DeepCopy(FQuestObject& Param) const
{
	Param.SetQuest(QuestSubject);
}

void FQuestObject::SetQuest(const TScriptInterface<IQuestSubject>& InQuestSubject)
{
	QuestSubject = InQuestSubject;
}

void FQuestObject::CompleteQuest()
{
	QuestSubject = nullptr;
}

void FQuestObject::DisposeQuest()
{
	QuestSubject = nullptr;
}
