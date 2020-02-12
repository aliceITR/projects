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
    public class AssignCategoriesController : Controller
    {
        private readonly ApplicationDbContext db = new ApplicationDbContext();

        [Authorize]
        public ActionResult Index(int CurryId)
        {
            CurryMVCv3.ViewModels.AssignCategoryViewModel model = CreateAssignCategoryViewModel(CurryId);

            if (model.CurryId == 0)
            {
                return HttpNotFound();
            }
            return View(model);
        }

        [HttpPost]
        public ActionResult Index(int CurryId, int CategoryId)
        {
            if (CurryId == 0)
            {
                return HttpNotFound();
            }
            if (ModelState.IsValid && CategoryId > 0)
            {
                //write a LINQ query to select a movie with an ID that is the same as the parameter MovieId                
                Curry movie = db.Curries.First(val => val.ID == CurryId);
                if (movie == null)
                {
                    return HttpNotFound();
                }

                //write a LINQ query to select a category with an ID that is the same as the parameter CategoryId             
                Ingredients category = db.Ingredients.First(val => val.ID == CategoryId);
                if (category == null)
                {
                    return HttpNotFound("Category not found");
                }
                movie.Categories.Add(category);
                db.SaveChanges();
            }

            CurryMVCv3.ViewModels.AssignCategoryViewModel model = CreateAssignCategoryViewModel(CurryId);

            return View(model);
        }

        [Authorize]
        public ActionResult Delete(int CategoryId, int CurryId)
        {
            if (CategoryId > 0)
            {
                //write a LINQ query to select a category with an ID that is the same as the parameter CategoryId   
                Ingredients category = db.Ingredients.First(val => val.ID == CategoryId);
                if (category == null)
                {
                    return HttpNotFound();
                }
                if (CurryId > 0)
                {
                    //write a LINQ query to select a movie with an ID that is the same as the parameter MovieId  
                    Curry curry = db.Curries.First(val => val.ID == CurryId);
                    if (curry == null)
                    {
                        return HttpNotFound();
                    }
                    curry.Categories.Remove(category);
                    db.SaveChanges();
                }
            }
            return View("Index", CreateAssignCategoryViewModel(CurryId));
        }


        [Authorize]
        private AssignCategoryViewModel CreateAssignCategoryViewModel(int CurryId)
        {
            CurryMVCv3.ViewModels.AssignCategoryViewModel model = new CurryMVCv3.ViewModels.AssignCategoryViewModel();

            //write a LINQ query to select the first movie with an ID that is the same as the parameter MovieId
            Curry curry = db.Curries.First(val => val.ID == CurryId);


            if (curry == null)
            {
                model.CurryId = 0;
            }
            else
            {
                model.CurryId = curry.ID;
                model.CurryName = curry.CurryRecipeName;


                //Get the list of assigned category ids for this product
                IList<int> assignedCategoryIds = curry.Categories.Select(c => c.ID).ToList();


                //Get a list of assigned category objects
                IList<Ingredients> assignedCategories = db.Ingredients.Where(c => assignedCategoryIds.Contains(c.ID)).ToList();

                model.AssociatedCategories = assignedCategories;

                //Get a list of available category objects that could be assigned
                IList<Ingredients> availableCategories = db.Ingredients.Where(c => !assignedCategoryIds.Contains(c.ID)).ToList();

                //add a default value to show at the top of the list
                availableCategories.Insert(0, new Ingredients() { ID = 0, CategoryName = "--- Select a Category ---" });

                //Convert to SelectList object for use with (bind to) drop down list.
                model.Categories = new SelectList(availableCategories, "ID", "CategoryName", 0);
            }

            return model;
        }

        // GET: AssignCategories/Details/5
       

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
