// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
class IQuestSubject;

class FQuestObject
{
public:
	FQuestObject() = default;
	FQuestObject(const FQuestObject&) = delete;
	FQuestObject& operator=(const FQuestObject&) = delete;
	void DeepCopy(FQuestObject& Param) const;
	void SetQuest(const TScriptInterface<IQuestSubject>& InQuestSubject);
	void CompleteQuest();
	void DisposeQuest();

public:
	TScriptInterface<IQuestSubject> QuestSubject;
};