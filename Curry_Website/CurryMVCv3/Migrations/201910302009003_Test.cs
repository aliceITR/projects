namespace CurryMVCv3.Migrations
{
    using System;
    using System.Data.Entity.Migrations;
    
    public partial class Test : DbMigration
    {
        public override void Up()
        {
            CreateTable(
                "dbo.Curries",
                c => new
                    {
                        ID = c.Int(nullable: false, identity: true),
                        CurryRecipeName = c.String(),
                        IsVegitarian = c.Boolean(nullable: false),
                        Origin = c.String(),
                        FeedsXPeople = c.Int(nullable: false),
                        ExpectedCost = c.Double(nullable: false),
                        TimeToCook = c.Int(nullable: false),
                        Color = c.String(),
                        CurryTypeID = c.Int(nullable: false),
                    })
                .PrimaryKey(t => t.ID)
                .ForeignKey("dbo.CurryTypes", t => t.CurryTypeID, cascadeDelete: true)
                .Index(t => t.CurryTypeID);
            
            CreateTable(
                "dbo.CurryTypes",
                c => new
                    {
                        ID = c.Int(nullable: false, identity: true),
                        CurryTypeName = c.String(),
                        LastModified = c.DateTime(nullable: false),
                        LastModifiedBy = c.String(),
                    })
                .PrimaryKey(t => t.ID);
            
        }
        
        public override void Down()
        {
            DropForeignKey("dbo.Curries", "CurryTypeID", "dbo.CurryTypes");
            DropIndex("dbo.Curries", new[] { "CurryTypeID" });
            DropTable("dbo.CurryTypes");
            DropTable("dbo.Curries");
        }
    }
}
