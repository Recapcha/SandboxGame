// Shoot Them Up Game, All Right Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class STUTarget : TargetRules
{
	public STUTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "STU" } );
	}
}
