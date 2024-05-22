// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class RTStest : ModuleRules
{
	public RTStest(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG" , "AIModule","GameplayTasks", "NavigationSystem"});

		PrivateDependencyModuleNames.AddRange(new string[] {"CashGen", "UnrealFastNoisePlugin"});
		//TODO 사용버전에서 풀필요 있음.
		OptimizeCode = CodeOptimization.Never;

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
