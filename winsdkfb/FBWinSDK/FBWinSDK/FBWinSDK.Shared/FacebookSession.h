//******************************************************************************
//
// Copyright (c) 2015 Microsoft Corporation. All rights reserved.
//
// This code is licensed under the MIT License (MIT).
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
//******************************************************************************

#pragma once

#include "FacebookAccessTokenData.h"
#include "FacebookResult.h"
#include "FBUser.h"
#include "FacebookDialog.xaml.h"

namespace Facebook
{
    //! Default audience for sharing.  3 available settings
    public enum class SessionDefaultAudience
    {
        SessionDefaultAudienceNone = 0,
        SessionDefaultAudienceOnlyMe = 10,
        SessionDefaultAudienceFriends = 20,
        SessionDefaultAudienceEveryone = 30
    };

    //! Specifies behavior of login for web view vs. app
    public enum class SessionLoginBehavior
    {
        SessionLoginBehaviorWithFallbackToWebView = 0,
        SessionLoginBehaviorWithNoFallbackToWebView = 1,
        SessionLoginBehaviorForcingWebView = 2
    };

    ref class FBSession;

    /*!\brief The main object for the SDK, repository for access token, etc.
     */
    public ref class FBSession sealed
    {
        public:

            //! Facebook App ID
            property Platform::String^ FBAppId
            {
                Platform::String^ get();
                void set(Platform::String^);
            }

            //! Windows App ID
            property Platform::String^ WinAppId
            {
                Platform::String^ get();
                void set(Platform::String^);
            }

            //! Response from FB App
            property Platform::String^ AppResponse 
            {
                Platform::String^ get();
            }

            //! Returns whether session (user) is logged in
            property bool LoggedIn
            {
                bool get();
            }

            //! Access token data 
            property Facebook::FBAccessTokenData^ AccessTokenData
            {
                Facebook::FBAccessTokenData^ get();
                void set(FBAccessTokenData^ value);
            }

            //! Returns the list of permissions
            property Windows::Foundation::Collections::IVectorView<Platform::String^>^ Permissions
            {
                Windows::Foundation::Collections::IVectorView<Platform::String^>^ get();
            }

            //! Expiration date/time of active session
            property Windows::Foundation::DateTime Expires
            {
                Windows::Foundation::DateTime get();
                void set(Windows::Foundation::DateTime);
            }

            //! Is session expired?
            property bool IsExpired
            {
                bool get();
            }

            //! Request a new permission
            void AddPermission(
                Platform::String^ permission
                );

            void ResetPermissions(
                );

            //! FBSession is a singleton object - ActiveSession is the way to
            // acquire a reference to the object.
            static property FBSession^ ActiveSession
            {
                FBSession^ get()
                {
                    static FBSession^ activeFBSession = ref new FBSession();
                    return activeFBSession;
                }
            }

            //! Clear all login information, e.g. user info, token string, etc.
            Windows::Foundation::IAsyncAction^ Logout();

            //! User info - valid after successful login
            property Facebook::Graph::FBUser^ User
            {
                Facebook::Graph::FBUser^ get();
            }

            //! Launch 'feed' dialog, to post to user's timeline
            Windows::Foundation::IAsyncOperation<FBResult^>^ ShowFeedDialog(
                Windows::Foundation::Collections::PropertySet^ Parameters
                );

            //! Launch 'request' dialog, to send app
            Windows::Foundation::IAsyncOperation<FBResult^>^ ShowRequestsDialog(
                Windows::Foundation::Collections::PropertySet^ Parameters
                );

            Platform::String^ PermissionsToString(
                );

            Windows::Foundation::IAsyncOperation<FBResult^>^ LoginAsync(
                );

        private:
            FBSession();
           
			~FBSession();

            Windows::Foundation::Uri^ BuildLoginUri(
                );

            Platform::String^ GetRedirectUriString(
                );

            concurrency::task<FBResult^> GetUserInfo(
                Facebook::FBAccessTokenData^ TokenData
                );

            void ParseOAuthResponse(
                Windows::Foundation::Uri^ ResponseUri
                );

            Windows::Foundation::IAsyncOperation<Windows::Storage::IStorageItem^>^ 
            MyTryGetItemAsync(
                Windows::Storage::StorageFolder^ folder,
                Platform::String^ itemName
                );

            concurrency::task<FBResult^> CheckForExistingToken(
                );

            void TrySaveTokenData(
                );

            Windows::Foundation::IAsyncAction^ TryDeleteTokenData(
                );

            concurrency::task<FBResult^> GetAppPermissions(
                );

			concurrency::task<Facebook::FBResult^>
				ProcessAuthResult(
				Windows::Security::Authentication::Web::WebAuthenticationResult^ authResult
				);

			concurrency::task<Facebook::FBResult^> TryGetUserInfoAfterLogin(
				Facebook::FBResult^ loginResult
				);

			concurrency::task<Facebook::FBResult^> TryGetAppPermissionsAfterLogin(
				Facebook::FBResult^ loginResult
				);

            concurrency::task<FBResult^> RunOAuthOnUiThread(
                );

            concurrency::task<FBResult^> RunWebViewLoginOnUIThread(
                );

            concurrency::task<FBResult^> ShowLoginDialog(
                );

            concurrency::task<FBResult^> TryLoginViaWebView(
                );

            concurrency::task<FBResult^> TryLoginViaWebAuthBroker(
                );

            int64 SecondsTilTokenExpires(
                Windows::Foundation::DateTime Expiration
                );

            Platform::String^ m_FBAppId;
            Platform::String^ m_WinAppId;
            bool m_loggedIn;
            Platform::String^ m_AppResponse;
            Facebook::FBAccessTokenData^ m_AccessTokenData;
            Platform::Collections::Vector<Platform::String^>^ m_permissions;
            Windows::Foundation::DateTime m_Expires;
            Facebook::Graph::FBUser^ m_user;
			concurrency::task<Facebook::FBResult^> m_loginTask;
            Facebook::FacebookDialog^ m_dialog;
            BOOL m_showingDialog;
    };
}
