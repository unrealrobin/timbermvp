// Property of Paracosm.

#include "Subsystems/Online/Login.h"
#include "Online/OnlineServices.h"
#include "Online/OnlineAsyncOpHandle.h"
#include "Online/Auth.h"
#include "Online/UserInfo.h"
#include "Online/ExternalUI.h"
#include "Online/AuthErrors.h"
#include "Online/OnlineResult.h"

void ULogin::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void ULogin::LoginAuto()
{
	using namespace UE::Online;

	IOnlineServicesPtr Services = GetServices();
	if (!Services.IsValid()) return;
	GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Green, TEXT("Online Service Valid."));

	IAuthPtr Auth = Services->GetAuthInterface();
	if (!Auth.IsValid()) return;
	GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Green, TEXT("Auth Interface Valid."));
	
	IUserInfoPtr UserInfo = Services->GetUserInfoInterface();
	if (!UserInfo.IsValid()) return;
	GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Green, TEXT("User Info Interface Valid.."));
	
	FAuthLogin::Params LoginParams;
	LoginParams.PlatformUserId = FPlatformUserId::CreateFromInternalId(0);
	LoginParams.CredentialsType = LoginCredentialsType::AccountPortal;

	Auth->Login(MoveTemp(LoginParams)).OnComplete([UserInfo](const TOnlineResult<FAuthLogin>& Result)
	{
		if (Result.IsOk())
		{
			GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Green, TEXT("Login successful."));
			
			FAccountId LoggedInPlayerAccountId = Result.GetOkValue().AccountInfo->AccountId;

			/*After Retrieving the Account ID we want to Display the Users Display Name*/
			FGetUserInfo::Params GetUserInfoParams;
			GetUserInfoParams.AccountId = LoggedInPlayerAccountId;
			GetUserInfoParams.LocalAccountId = LoggedInPlayerAccountId;
			
			bool bIsOk = UserInfo->GetUserInfo(MoveTemp(GetUserInfoParams)).IsOk();
			if (bIsOk)
			{
				FString LoggedInUserName = UserInfo->GetUserInfo(MoveTemp(GetUserInfoParams)).GetOkValue().UserInfo->DisplayName;
				GEngine->AddOnScreenDebugMessage(1, 10, FColor::Green, FString::Printf(TEXT("Logged in as: %s"), *LoggedInUserName));
			}
			
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Login failed: %s"), *Result.GetErrorValue().GetLogString());
		}
	});
	
}



FPlatformUserId ULogin::GetPlatformUserId()
{
	FPlatformUserId PlatformUserId =  PLATFORMUSERID_NONE;
	return PlatformUserId;
}
