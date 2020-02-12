using System;
using System.Collections.Generic;
using System.Data;
using System.Data.Entity;
using System.Linq;
using System.Net;
using System.Web;
using System.Web.Mvc;
using CurryMVCv3.Models;

namespace CurryMVCv3.Controllers

{
    public class CurriesController : Controller
    {
         private ApplicationDbContext db = new ApplicationDbContext();

        // GET: Curries
        public ActionResult Index()
        {
            var curries = db.Curries.Include(c => c.CurryType);
            return View(curries.ToList());
        }

        // GET: Curries/Details/5
        public ActionResult Details(int? id)
        {
            if (id == null)
            {
                return new HttpStatusCodeResult(HttpStatusCode.BadRequest);
            }
            Curry curry = db.Curries.Find(id);
            if (curry == null)
            {
                return HttpNotFound();
            }
            return View(curry);
        }

        // GET: Curries/Create
        [Authorize]
        public ActionResult Create()
        {
            ViewBag.CurryTypeID = new SelectList(db.CurryTypes, "ID", "CurryTypeName");
            return View();
        }

        // POST: Curries/Create
        // To protect from overposting attacks, please enable the specific properties you want to bind to, for 
        // more details see https://go.microsoft.com/fwlink/?LinkId=317598.
        [HttpPost]
        [ValidateAntiForgeryToken]
        [Authorize]
        public ActionResult Create([Bind(Include = "ID,CurryRecipeName,IsVegitarian,Origin,FeedsXPeople,ExpectedCost,TimeToCook,Color,CurryTypeID")] Curry curry)
        {
            if (ModelState.IsValid)
            {
                db.Curries.Add(curry);
                db.SaveChanges();
                return RedirectToAction("Index");
            }

            ViewBag.CurryTypeID = new SelectList(db.CurryTypes, "ID", "CurryTypeName", curry.CurryTypeID);
            return View(curry);
        }

        // GET: Curries/Edit/5
        [Authorize]
        public ActionResult Edit(int? id)
        {
            if (id == null)
            {
                return new HttpStatusCodeResult(HttpStatusCode.BadRequest);
            }
            Curry curry = db.Curries.Find(id);
            if (curry == null)
            {
                return HttpNotFound();
            }
            ViewBag.CurryTypeID = new SelectList(db.CurryTypes, "ID", "CurryTypeName", curry.CurryTypeID);
            return View(curry);
        }

        // POST: Curries/Edit/5
        // To protect from overposting attacks, please enable the specific properties you want to bind to, for 
        // more details see https://go.microsoft.com/fwlink/?LinkId=317598.
        [HttpPost]
        [ValidateAntiForgeryToken]
        [Authorize]
        public ActionResult Edit([Bind(Include = "ID,CurryRecipeName,IsVegitarian,Origin,FeedsXPeople,ExpectedCost,TimeToCook,Color,CurryTypeID")] Curry curry)
        {
            if (ModelState.IsValid)
            {
                db.Entry(curry).State = EntityState.Modified;
                db.SaveChanges();
                return RedirectToAction("Index");
            }
            ViewBag.CurryTypeID = new SelectList(db.CurryTypes, "ID", "CurryTypeName", curry.CurryTypeID);
            return View(curry);
        }

        // GET: Curries/Delete/5
        [Authorize]
        public ActionResult Delete(int? id)
        {
            if (id == null)
            {
                return new HttpStatusCodeResult(HttpStatusCode.BadRequest);
            }
            Curry curry = db.Curries.Find(id);
            if (curry == null)
            {
                return HttpNotFound();
            }
            return View(curry);
        }

        // POST: Curries/Delete/5
        [HttpPost, ActionName("Delete")]
        [ValidateAntiForgeryToken]
        [Authorize]
        public ActionResult DeleteConfirmed(int id)
        {
            Curry curry = db.Curries.Find(id);
            db.Curries.Remove(curry);
            db.SaveChanges();
            return RedirectToAction("Index");
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
