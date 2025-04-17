// Property of Paracosm.

#include "Subsystems/Online/Login.h"
#include "Online/OnlineServices.h"
#include "Online/OnlineAsyncOpHandle.h"
#include "Online/Auth.h"
#include "Online/UserInfo.h"
#include "Online/AuthErrors.h"
#include "Online/OnlineResult.h"

void ULogin::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	//Handles setting up of all connections
	InitializeOnlineServices();
}

void ULogin::LoginAuto()
{
	if (OnlineService.IsValid() && AuthService.IsValid())
	{
		FAuthLogin::Params LoginParams;
		LoginParams.PlatformUserId = FPlatformUserId::CreateFromInternalId(0);
		LoginParams.CredentialsType = LoginCredentialsType::AccountPortal;

		AuthService->Login(MoveTemp(LoginParams)).OnComplete([this](const TOnlineResult<FAuthLogin>& Result)
		{
			if (Result.IsOk())
			{
				GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Green, TEXT("Login successful."));

				//Saving Struct Locally for Easy Access.
				UserInfo.AccountInfo = *Result.GetOkValue().AccountInfo;

				GetOnlineUserDisplayName();

				//Broadcasting to other systems that the player is logged in. (Start Menu)
				IsPlayerLoggedIn.Broadcast(true);
				
			}
		});
	}
}

FString ULogin::GetOnlineUserDisplayName()
{
	/*After Retrieving the Account ID we want to Display the Users Display Name*/
	if (UserInfo.AccountInfo.AccountId.IsValid() )
	{
		FAccountId PlayerAccountId = UserInfo.AccountInfo.AccountId;
		//UE_LOG(LogTemp, Warning, TEXT("Player Account ID: %s"), *PlayerAccountId.ToString());
		FGetUserInfo::Params GetUserInfoParams;
		GetUserInfoParams.AccountId = PlayerAccountId;
		GetUserInfoParams.LocalAccountId = PlayerAccountId;
		bool bIsOk = UserInfoService->GetUserInfo(MoveTemp(GetUserInfoParams)).IsOk();

		if (bIsOk)
		{
			UserInfo.OnlineUserDisplayName = UserInfoService->GetUserInfo(MoveTemp(GetUserInfoParams)).GetOkValue().UserInfo->DisplayName;
			FString Name = UserInfo.OnlineUserDisplayName;
			GEngine->AddOnScreenDebugMessage(1, 10, FColor::Green, FString::Printf(TEXT("Logged in as: %s"), *Name));
			return Name;
		}
	
		GEngine->AddOnScreenDebugMessage(1, 10, FColor::Green, TEXT("Error retrieving Display Name"));
		return TEXT("Error");
	
	}

	UE_LOG(LogTemp, Error, TEXT("Player Account ID is not valid."));
	return TEXT("Error");;

						
}

FPlatformUserId ULogin::GetPlatformUserId()
{
	FPlatformUserId PlatformUserId =  PLATFORMUSERID_NONE;
	return PlatformUserId;
}

void ULogin::InitializeOnlineServices()
{
	using namespace UE::Online;

	OnlineService = GetServices();
	if (OnlineService.IsValid())
	{
		AuthService = OnlineService->GetAuthInterface();
		if (AuthService.IsValid())
		{
			UserInfoService = OnlineService->GetUserInfoInterface();
			if (UserInfoService.IsValid())
			{
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(1, 5.0, FColor::Green, TEXT("Online Services Initialized."));
				}
			}
		}
	}
}
