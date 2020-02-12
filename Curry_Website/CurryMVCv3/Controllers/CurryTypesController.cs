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
    public class CurryTypesController : Controller
    {
        private ApplicationDbContext db = new ApplicationDbContext();

        // GET: CurryTypes
        public ActionResult Index()
        {
            return View(db.CurryTypes.ToList());
        }

        // GET: CurryTypes/Details/5
        public ActionResult Details(int? id)
        {
            if (id == null)
            {
                return new HttpStatusCodeResult(HttpStatusCode.BadRequest);
            }
            CurryTypes curryTypes = db.CurryTypes.Find(id);
            if (curryTypes == null)
            {
                return HttpNotFound();
            }
            return View(curryTypes);
        }

        // GET: CurryTypes/Create
        [Authorize]
        public ActionResult Create()
        {
            CurryTypes food = new CurryTypes();

            food.LastModified = DateTime.Now;
            return View(food);
        }

        // POST: CurryTypes/Create
        // To protect from overposting attacks, please enable the specific properties you want to bind to, for 
        // more details see https://go.microsoft.com/fwlink/?LinkId=317598.
        [HttpPost]
        [ValidateAntiForgeryToken]
        [Authorize]
        public ActionResult Create([Bind(Include = "ID,CurryTypeName,LastModified,LastModifiedBy")] CurryTypes curryTypes)
        {
            if (ModelState.IsValid)
            {
                db.CurryTypes.Add(curryTypes);
                db.SaveChanges();
                return RedirectToAction("Index");
            }

            return View(curryTypes);
        }

        // GET: CurryTypes/Edit/5
        [Authorize]
        public ActionResult Edit(int? id)
        {
            if (id == null)
            {
                return new HttpStatusCodeResult(HttpStatusCode.BadRequest);
            }
            CurryTypes curryTypes = db.CurryTypes.Find(id);
            if (curryTypes == null)
            {
                return HttpNotFound();
            }
            return View(curryTypes);
        }

        // POST: CurryTypes/Edit/5
        // To protect from overposting attacks, please enable the specific properties you want to bind to, for 
        // more details see https://go.microsoft.com/fwlink/?LinkId=317598.
        [HttpPost]
        [ValidateAntiForgeryToken]
        [Authorize]
        public ActionResult Edit([Bind(Include = "ID,CurryTypeName,LastModified,LastModifiedBy")] CurryTypes curryTypes)
        {
            if (ModelState.IsValid)
            {
                db.Entry(curryTypes).State = EntityState.Modified;
                db.SaveChanges();
                return RedirectToAction("Index");
            }
            return View(curryTypes);
        }

        // GET: CurryTypes/Delete/5
        [Authorize]
        public ActionResult Delete(int? id)
        {
            if (id == null)
            {
                return new HttpStatusCodeResult(HttpStatusCode.BadRequest);
            }
            CurryTypes curryTypes = db.CurryTypes.Find(id);
            if (curryTypes == null)
            {
                return HttpNotFound();
            }
            return View(curryTypes);
        }

        // POST: CurryTypes/Delete/5
        [HttpPost, ActionName("Delete")]
        [ValidateAntiForgeryToken]
        [Authorize]
        public ActionResult DeleteConfirmed(int id)
        {
            CurryTypes curryTypes = db.CurryTypes.Find(id);
            db.CurryTypes.Remove(curryTypes);
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
