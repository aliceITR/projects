namespace CurryMVCv3.Migrations
{
    using System;
    using System.Data.Entity.Migrations;
    
    public partial class ZZZZZZZZZ : DbMigration
    {
        public override void Up()
        {
            AddColumn("dbo.CurryTypes", "SearchByTypeViewModel_ID", c => c.Int());
            CreateIndex("dbo.CurryTypes", "SearchByTypeViewModel_ID");
            AddForeignKey("dbo.CurryTypes", "SearchByTypeViewModel_ID", "dbo.Curries", "ID");
        }
        
        public override void Down()
        {
            DropForeignKey("dbo.CurryTypes", "SearchByTypeViewModel_ID", "dbo.Curries");
            DropIndex("dbo.CurryTypes", new[] { "SearchByTypeViewModel_ID" });
            DropColumn("dbo.CurryTypes", "SearchByTypeViewModel_ID");
        }
    }
}
