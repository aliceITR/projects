using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.ComponentModel.DataAnnotations;

namespace CurryMVCv3.Models
{
    public class CurryTypes
    {
        public int ID { get; set; }
        [Display(Name = "Type")]
        [MaxLength(64, ErrorMessage = "Maximum length of 64 characters")]
        [Required(ErrorMessage = "Please enter a Type.")]
        public string CurryTypeName { get; set; }
        public DateTime LastModified { get; set; } 
        public string LastModifiedBy { get; set; }
    }
}