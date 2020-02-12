namespace CurryMVCv3.Migrations
{
    using System;
    using System.Data.Entity.Migrations;
    
    public partial class AddProp1 : DbMigration
    {
        public override void Up()
        {
            DropColumn("dbo.Curries", "Done");
        }
        
        public override void Down()
        {
            AddColumn("dbo.Curries", "Done", c => c.Boolean(nullable: false));
        }
    }
}
