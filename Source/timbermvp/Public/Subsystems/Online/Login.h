// Property of Paracosm.

#pragma once

#include "CoreMinimal.h"
#include "Online/Auth.h"
#include "Online/OnlineServices.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Login.generated.h"

using namespace UE::Online;

USTRUCT(BlueprintType)
struct FOnlineUserInfo
{
	GENERATED_BODY()

public:

	FAccountInfo AccountInfo;

	FString OnlineUserDisplayName = TEXT("Display Name Not Set.");
	
};

/**
 * 
 */
UCLASS(config = Engine, defaultconfig)
class TIMBERMVP_API ULogin : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	using FAccountId = TOnlineId<OnlineIdHandleTags::FAccount>;
	/*
	 * Broadcasts to other Systems whether the Local Player is Logged in and Authenticated
	 */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FIsPlayerLoggedIn, bool, bIsPlayerLoggedIn);
	FIsPlayerLoggedIn IsPlayerLoggedIn;

	void Initialize(FSubsystemCollectionBase& Collection) override;
public:

	UPROPERTY()
	FOnlineUserInfo UserInfo = FOnlineUserInfo();
	
	UFUNCTION(BlueprintCallable, Category = "Login")
	void LoginAuto();

	UFUNCTION(BlueprintCallable, Category = "Login")
	FString GetOnlineUserDisplayName();
	
private:
	void InitializeOnlineServices();

	IOnlineServicesPtr OnlineService = nullptr;
	
	IAuthPtr AuthService = nullptr;
	
	IUserInfoPtr UserInfoService = nullptr;
	
	FPlatformUserId GetPlatformUserId();
};
