using CurryMVCv3.Models;
using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.Linq;
using System.Web;
using System.Web.Mvc;

namespace CurryMVCv3.ViewModels
{
    public class AssignCategoryViewModel
    {
        public int CurryId { get; set; }

        [Required, Range(1, int.MaxValue, ErrorMessage = "Please select a category.")]
        public int CategoryId { get; set; }

        public string CurryName { get; set; }

        public IList<Ingredients> AssociatedCategories { get; set; }

        public SelectList Categories { get; set; }

        public IEnumerable<SelectListItem> AllCategories { get; set; }


        }

    }
