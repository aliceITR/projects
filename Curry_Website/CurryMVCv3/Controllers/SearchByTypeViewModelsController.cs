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
    public class SearchByTypeViewModelsController : Controller
    {
        private ApplicationDbContext db = new ApplicationDbContext();

        // GET: SearchByTypeViewModels
        public ActionResult Index()
        {
            ViewBag.CurryTypeID = new SelectList(db.CurryTypes, "ID", "CurryTypeName");
            var curries = db.Curries.Include(s => s.CurryType);
            return View(curries.ToList());
        }
        [HttpPost]
        public ActionResult Index(int CurryTypeID)
        {
            SearchByTypeViewModel model = new SearchByTypeViewModel();
            model.CurryTypes = db.CurryTypes.ToList();
            ViewBag.CurryTypeID = new SelectList(model.CurryTypes, "ID", "CurryTypeName");
            var curries = db.Curries.Where(a => a.CurryTypeID == CurryTypeID);
            return View(curries.ToList());
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
