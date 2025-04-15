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

void ULogin::AuthenticateUser()
{
	//Use the login popup to retrieve the Credentials token. Use callback to call the LoginUser function
	//Passing in the PlatformUserId and the CredentialsToken
}

void ULogin::ShowExternalUI()
{
	const UE::Online::IOnlineServicesPtr OnlineServices = UE::Online::GetServices();
	const UE::Online::IExternalUIPtr ExternalUI = OnlineServices->GetExternalUIInterface();

	ExternalUI->ShowLoginUI(UE::Online::FExternalUIShowLoginUI::Params()).OnComplete([](const UE::Online::TOnlineResult<UE::Online::FExternalUIShowLoginUI>& Result)
	{
		if (Result.IsOk())
		{
			const TSharedRef<UE::Online::FAccountInfo> AccountInfo = Result.GetOkValue().AccountInfo;
			// AccountInfo object is now accessible
			UE_LOG(LogTemp, Warning, TEXT("Login successful for user: %s"), *ToString(AccountInfo->AccountId));
		}
		else
		{
			UE::Online::FOnlineError Error = Result.GetErrorValue();
			// Error can now be processed
		}
	});
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

void ULogin::LoginUser(::FPlatformUserId PlatformUserId, FString CredentialsToken)
{
	UE::Online::IOnlineServicesPtr OnlineServices = UE::Online::GetServices();
	UE::Online::IAuthPtr AuthInterface = OnlineServices->GetAuthInterface();
	
	UE::Online::FAuthLogin::Params LoginParams;
	LoginParams.CredentialsType = UE::Online::LoginCredentialsType::AccountPortal;
	LoginParams.PlatformUserId = PlatformUserId;

	//Constructing a Variant to work with the CreditalToken Type Needs.
	LoginParams.CredentialsToken = TVariant<FString, UE::Online::FExternalAuthToken>(TInPlaceType<FString>(), CredentialsToken);;
	
	AuthInterface->Login(MoveTemp(LoginParams)).OnComplete([](const UE::Online::TOnlineResult<UE::Online::FAuthLogin>& Result)
				{
					if(Result.IsOk())
					{
						const TSharedRef<UE::Online::FAccountInfo> AccountInfo = Result.GetOkValue().AccountInfo;
						// AccountInfo object is now accessible
						UE_LOG(LogTemp, Warning, TEXT("Login successful for user: %s"), *ToString(AccountInfo->AccountId));
					}
					else
					{
						UE::Online::FOnlineError Error = Result.GetErrorValue();
						// Error can now be processed
					}
				});
 
	UE_LOG(LogTemp, Warning, TEXT("Called Handle Login Function"));
}

FPlatformUserId ULogin::GetPlatformUserId()
{
	FPlatformUserId PlatformUserId =  PLATFORMUSERID_NONE;
	return PlatformUserId;
}
