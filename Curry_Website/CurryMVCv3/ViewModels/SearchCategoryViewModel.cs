using CurryMVCv3.Models;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace CurryMVCv3.ViewModels
{
    public class SearchCategoryViewModel:Curry
    {
        public IList<Ingredients> AllCategories { get; set; }
    }
}