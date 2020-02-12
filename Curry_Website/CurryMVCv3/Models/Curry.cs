using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.ComponentModel.DataAnnotations;

namespace CurryMVCv3.Models
{
    public class Curry
    {
        public Curry()
        {
            this.Categories = new HashSet<Ingredients>();
        }

        public int ID { get; set; }

        [Display(Name = "Curry Recipe Name")]
        [DataType(DataType.Text)]
        [Required(ErrorMessage = "Please enter a Recipe Name.")]
        public string CurryRecipeName { get; set; }
        [Display(Name = "Is it Vegitarian")]
        public bool IsVegitarian { get; set; }
        [Display(Name = "Curry's Origin")]
        [DataType(DataType.Text)]
        public string Origin { get; set; }
        [Display(Name = "How Many people does it feed")]
        [DataType(DataType.Text)]
        [Required(ErrorMessage = "Please enter an amount of people that this curry feeds.")]
        public int FeedsXPeople { get; set; }
        [Display(Name = "How much does it cost")]
        [DataType(DataType.Currency)]
        public double ExpectedCost { get; set; }
        [Display(Name = "How long does it take (in mins)")]
        [DataType(DataType.Duration)]
        public int TimeToCook { get; set; } //In minutes
        [Display(Name = "Curry's Color")]
        [DataType(DataType.Text)]
        public string Color { get; set; }

        [Display(Name = "Curry's Type")]  
        public int CurryTypeID { get; set; }

        public virtual CurryTypes CurryType { get; set; }

        public virtual ICollection<Ingredients> Categories { get; set; }
    }
}