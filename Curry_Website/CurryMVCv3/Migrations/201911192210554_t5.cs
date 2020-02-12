namespace CurryMVCv3.Migrations
{
    using System;
    using System.Data.Entity.Migrations;
    
    public partial class t5 : DbMigration
    {
        public override void Up()
        {
            CreateTable(
                "dbo.Ingredients",
                c => new
                    {
                        ID = c.Int(nullable: false, identity: true),
                        CategoryName = c.String(nullable: false, maxLength: 64),
                        LastModified = c.DateTime(),
                        LastModifiedBy = c.String(),
                    })
                .PrimaryKey(t => t.ID);
            
            CreateTable(
                "dbo.CurryCategory",
                c => new
                    {
                        CurryId = c.Int(nullable: false),
                        CategoryId = c.Int(nullable: false),
                    })
                .PrimaryKey(t => new { t.CurryId, t.CategoryId })
                .ForeignKey("dbo.Curries", t => t.CurryId, cascadeDelete: true)
                .ForeignKey("dbo.Ingredients", t => t.CategoryId, cascadeDelete: true)
                .Index(t => t.CurryId)
                .Index(t => t.CategoryId);
            
        }
        
        public override void Down()
        {
            DropForeignKey("dbo.CurryCategory", "CategoryId", "dbo.Ingredients");
            DropForeignKey("dbo.CurryCategory", "CurryId", "dbo.Curries");
            DropIndex("dbo.CurryCategory", new[] { "CategoryId" });
            DropIndex("dbo.CurryCategory", new[] { "CurryId" });
            DropTable("dbo.CurryCategory");
            DropTable("dbo.Ingredients");
        }
    }
}
