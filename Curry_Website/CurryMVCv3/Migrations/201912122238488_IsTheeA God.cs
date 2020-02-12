namespace CurryMVCv3.Migrations
{
    using System;
    using System.Data.Entity.Migrations;
    
    public partial class IsTheeAGod : DbMigration
    {
        public override void Up()
        {
            AddColumn("dbo.Curries", "Discriminator", c => c.String(nullable: false, maxLength: 128));
            AddColumn("dbo.Ingredients", "SearchCategoryViewModel_ID", c => c.Int());
            CreateIndex("dbo.Ingredients", "SearchCategoryViewModel_ID");
            AddForeignKey("dbo.Ingredients", "SearchCategoryViewModel_ID", "dbo.Curries", "ID");
        }
        
        public override void Down()
        {
            DropForeignKey("dbo.Ingredients", "SearchCategoryViewModel_ID", "dbo.Curries");
            DropIndex("dbo.Ingredients", new[] { "SearchCategoryViewModel_ID" });
            DropColumn("dbo.Ingredients", "SearchCategoryViewModel_ID");
            DropColumn("dbo.Curries", "Discriminator");
        }
    }
}
