using Microsoft.Owin;
using Owin;

[assembly: OwinStartupAttribute(typeof(CurryMVCv3.Startup))]
namespace CurryMVCv3
{
    public partial class Startup
    {
        public void Configuration(IAppBuilder app)
        {
            ConfigureAuth(app);
        }
    }
}
