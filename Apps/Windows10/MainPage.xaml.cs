using Facebook;
using Facebook.Graph;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.Security.Authentication.Web;
using Windows.UI.Popups;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;
using Windows.Web.Syndication;
using Windows10.Entities;

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

namespace Windows10
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainPage : Page
    {
        public MainPage()
        {
            this.InitializeComponent();
        }

        private async void OnLoginClicked(object sender, RoutedEventArgs e)
        {
            string SID = WebAuthenticationBroker.GetCurrentApplicationCallbackUri().ToString();
            string AppId = "858381294217135";

            FBSession session = FBSession.ActiveSession;
            session.FBAppId = AppId;
            session.WinAppId = SID;
            session.AddPermission("public_profile");

            FBResult result = await session.LoginAsync();
            if (result.Succeeded)
            {
                FBUser user = session.User;
                UserName.Text = user.Name;
                UserId.Text = user.Id;
                Profile.UserId = user.Id;
                UserProfile.Visibility = Visibility.Visible;
            }
        }

        private async void OnPostClicked(object sender, RoutedEventArgs e)
        {
            FBSession session = FBSession.ActiveSession;
            if (session.LoggedIn)
            {
                PropertySet parameters = new PropertySet();
                parameters.Add("title", "MSDN Italy blog");
                parameters.Add("link", "http://blogs.msdn.com/b/italy/");
                parameters.Add("description", "Il blog ufficiale di MSDN Italia");

                FBResult result = await session.ShowFeedDialog(parameters);
                string message;
                if (result.Succeeded)
                {
                    message = "Succeded";
                }
                else
                {
                    message = "Failed";
                }

                MessageDialog dialog = new MessageDialog(message);
                await dialog.ShowAsync();
            }
        }

        private async void OnGetInfoClicked(object sender, RoutedEventArgs e)
        {
            string endpoint = "/me";
            FBSingleValue value = new FBSingleValue(endpoint, null, Entities.Profile.FromJson);
            FBResult result = await value.Get();
            if (result.Succeeded)
            {
                Profile profile = result.Object as Profile;
                string name = profile?.Name;
                MessageDialog dialog = new MessageDialog(name);
                await dialog.ShowAsync();
            }
        }
    }
}
