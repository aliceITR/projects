namespace CurryMVCv3.Migrations
{
    using System;
    using System.Data.Entity.Migrations;
    
    public partial class Test2 : DbMigration
    {
        public override void Up()
        {
            AlterColumn("dbo.Curries", "CurryRecipeName", c => c.String(nullable: false));
            AlterColumn("dbo.CurryTypes", "CurryTypeName", c => c.String(nullable: false, maxLength: 64));
        }
        
        public override void Down()
        {
            AlterColumn("dbo.CurryTypes", "CurryTypeName", c => c.String());
            AlterColumn("dbo.Curries", "CurryRecipeName", c => c.String());
        }
    }
}
