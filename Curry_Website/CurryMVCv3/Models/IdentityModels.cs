using System.Data.Entity;
using System.Security.Claims;
using System.Threading.Tasks;
using Microsoft.AspNet.Identity;
using Microsoft.AspNet.Identity.EntityFramework;

namespace CurryMVCv3.Models
{
    // You can add profile data for the user by adding more properties to your ApplicationUser class, please visit https://go.microsoft.com/fwlink/?LinkID=317594 to learn more.
    public class ApplicationUser : IdentityUser
    {
        public async Task<ClaimsIdentity> GenerateUserIdentityAsync(UserManager<ApplicationUser> manager)
        {
            // Note the authenticationType must match the one defined in CookieAuthenticationOptions.AuthenticationType
            var userIdentity = await manager.CreateIdentityAsync(this, DefaultAuthenticationTypes.ApplicationCookie);
            // Add custom user claims here
            return userIdentity;
        }
    }

    public class ApplicationDbContext : IdentityDbContext<ApplicationUser>
    {
        public ApplicationDbContext()
            : base("DefaultConnection", throwIfV1Schema: false)
        {
        }

        public static ApplicationDbContext Create()
        {
            return new ApplicationDbContext();
        }

        protected override void OnModelCreating(DbModelBuilder modelBuilder)
        {
            base.OnModelCreating(modelBuilder);

            modelBuilder.Entity<Curry>()
                .HasMany(m => m.Categories)
                .WithMany(c => c.Curries)
                .Map(m =>
                {
                    m.ToTable("CurryCategory");
                    m.MapLeftKey("CurryId");
                    m.MapRightKey("CategoryId");
                });

        }





        public System.Data.Entity.DbSet<CurryMVCv3.Models.Curry> Curries { get; set; }

        public System.Data.Entity.DbSet<CurryMVCv3.Models.CurryTypes> CurryTypes { get; set; }

        public System.Data.Entity.DbSet<CurryMVCv3.Models.Ingredients> Ingredients { get; set; }
    }
}