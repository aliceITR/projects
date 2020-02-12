using System;
using System.Collections.Generic;
using System.Data;
using System.Data.Entity;
using System.Linq;
using System.Net;
using System.Web;
using System.Web.Mvc;
using CurryMVCv3.Models;
using CurryMVCv3.ViewModels;

namespace CurryMVCv3.Controllers
{
    public class SearchCategoryViewModelsController : Controller
    {
        private ApplicationDbContext db = new ApplicationDbContext();

        // GET: SearchCategoryViewModels
        public ActionResult Index()
        {
            ViewBag.CategoryID = new SelectList(db.Ingredients, "ID", "CategoryName");
            var curries = db.Curries.Include(s => s.CurryType);
            return View(curries.ToList());
        }
        [HttpPost]
        public ActionResult Index(int CategoryID)
        {
            SearchCategoryViewModel model = new SearchCategoryViewModel();
            model.AllCategories = db.Ingredients.ToList();
            ViewBag.CategoryID = new SelectList(model.AllCategories, "ID", "CategoryName");
            var curries = db.Ingredients.Find(CategoryID);
            return View(curries.Curries.ToList());
        }
        protected override void Dispose(bool disposing)
        {
            if (disposing)
            {
                db.Dispose();
            }
            base.Dispose(disposing);
        }
    }
}
